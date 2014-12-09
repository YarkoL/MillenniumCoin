#include "transactiondescdialog.h"
#include "ui_transactiondescdialog.h"

#include "transactiontablemodel.h"
#include "init.h" //need pwalletMain

#include <QModelIndex>

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
        err = " : ";
        err.prepend(QString(tx_id.ToString().c_str()));
        err.append(QString(retrieve.c_str()));
    } else {
         err = "No retrieval string for ";
         err.append(QString(tx_id.ToString().c_str()));
    }
    ui->detailText->setText(err);
}
