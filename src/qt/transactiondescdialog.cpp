#include "transactiondescdialog.h"
#include "ui_transactiondescdialog.h"

#include "transactiontablemodel.h"
#include "init.h"  //need pwalletMain
#include "net.h"
#include "bitcoinrpc.h"
#include <boost/algorithm/string.hpp>
#include "util.h"
#include <QModelIndex>
#include <QDebug>
#include <QtDebug>

TransactionDescDialog::TransactionDescDialog(const QModelIndex &idx, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionDescDialog)
{
    ui->setupUi(this);

    QString desc = idx.data(TransactionTableModel::LongDescriptionRole).toString();
    int status = idx.data(TransactionTableModel::StatusRole).toInt();

    //there must be a less laughable way to get the tx id than this :P
    QString hash = idx.data(TransactionTableModel::Hash).toString();
    std::string tx_id_str = hash.toUtf8().constData();
    tx_id = uint256(tx_id_str);

    ui->detailText->setHtml(desc);
    ui->retrieveButton->setEnabled(status == TransactionStatus::Escrow || status == TransactionStatus::Expiry );

    isEscrow = (status == TransactionStatus::Escrow);
    depth = idx.data(TransactionTableModel::Depth).toInt();
    connect(ui->retrieveButton, SIGNAL(clicked()), this, SLOT(retrieveTxHandler()));
}

TransactionDescDialog::~TransactionDescDialog()
{
    delete ui;
}

void TransactionDescDialog::retrieveTxHandler()
{
    std::string retrieve;
    QString err;
    std::string ret;


    if (pwalletMain->ReadRetrieveStringFromHashMap(tx_id, retrieve, isEscrow)) {

        qDebug() << QString(retrieve.c_str());

        std::vector<std::string> params;
        boost::split(params, retrieve, boost::is_any_of(" "));
        if (isEscrow ) {
            std::string const destination_address = params[0];
            uint256 const bind_txid = ParseHashV(params[1], "bind_txid");
            ret = CreateTransferExpiry(destination_address, bind_txid, depth);
            err = QString(ret.c_str());

            if (depth >= 5 && !err.startsWith(QString("OK!")) ) {
                try {

                       std::string const destination_address = params[0];
                       uint256 const sender_confirmtx_hash = ParseHashV(params[1], "sender_confirmtx_hash");
                       std::string const sender_tor_address = params[2];
                       uint64_t const sender_address_bind_nonce = toUint64(params[3]);
                       uint64_t const transfer_nonce =  toUint64(params[4]);
                       std::vector<unsigned char> const transfer_tx_hash = ParseHexV(params[5],"transfer_tx_hash");

                        ret = CreateTransferEscrow (
                                    destination_address,
                                    sender_confirmtx_hash,
                                    sender_tor_address,
                                    sender_address_bind_nonce,
                                    transfer_nonce,
                                    transfer_tx_hash,
                                    depth
                                    );
                        err = QString(ret.c_str());

                   }
                   catch (const std::exception& ex)
                   {
                        err = QString(ex.what());
                   }
            }

         } else {

            std::string const destination_address = params[0];
            uint256 const bind_txid = ParseHashV(params[1], "bind_txid");
            ret = CreateTransferExpiry(destination_address, bind_txid, depth);
            err = QString(ret.c_str());
        }
    }
    if (err.startsWith(QString("OK!"))) {
        //retrieval success, so delete entry in the map
        pwalletMain->DeleteRetrieveStringFromDB(tx_id);
    }
    ui->detailText->setText(err);
}
