#include "transactiondescdialog.h"
#include "ui_transactiondescdialog.h"

#include "transactiontablemodel.h"
#include "init.h"  //need pwalletMain
#include "net.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

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

    ui->retrieveButton->setEnabled(status == TransactionStatus::Escrow);
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
    if (pwalletMain->get_delegate_retrieve(tx_id, retrieve)) {

        qDebug() << QString(retrieve.c_str());

        std::vector<std::string> params;
        boost::split(params, retrieve, boost::is_any_of(" "));
        if (params.size() < 2) {
            err = "too few params";
        } else {
             try {
                    std::string const destination_address = params[0];
                    uint256 const sender_confirmtx_hash = uint256(params[1]);
                    std::string const sender_tor_address = params[2];
                    /*
                    std::stringstream ss;
                    ss << "12345678901234567890";

                    uint64_t n = 0;
                    ss >> n;
                    qDebug() << n;
                    */
                    boost::uint64_t const sender_address_bind_nonce = boost::lexical_cast<uint64_t>(params[3]);
                    boost::uint64_t const transfer_nonce =  boost::lexical_cast<uint64_t>(params[4]);
                    std::vector<unsigned char> const transfer_tx_hash = ParseHex(params[5]);
                    std::string err_str;

                     err_str = CreateTransferEscrow (
                                 destination_address,
                                 sender_confirmtx_hash,
                                 sender_tor_address,
                                 sender_address_bind_nonce,
                                 transfer_nonce,
                                 transfer_tx_hash
                                 );
                     err = QString(err_str.c_str());
                }
                catch (const std::exception& ex)
                {
                     err = QString(ex.what());
                }
            }
    } else {
         err = "No retrieval string for ";
         err.append(QString(tx_id.ToString().c_str()));
    }
    ui->detailText->setText(err);
}
