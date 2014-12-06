#include "transactiondescdialog.h"
#include "ui_transactiondescdialog.h"

#include "transactiontablemodel.h"

#include <QModelIndex>

TransactionDescDialog::TransactionDescDialog(const QModelIndex &idx, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionDescDialog)
{
    rec = static_cast<TransactionRecord*>(idx.internalPointer());
    ui->setupUi(this);

    QString desc = idx.data(TransactionTableModel::LongDescriptionRole).toString();
    ui->detailText->setHtml(desc);

    //ui->retrieveButton->setEnabled(false);
    connect(ui->retrieveButton, SIGNAL(clicked()), this, SLOT(retrieveClicked()));
}

TransactionDescDialog::~TransactionDescDialog()
{
    delete ui;
}

void TransactionDescDialog::retrieveClicked()
{

    QString hash = QString(rec->hash.ToString().c_str());
     ui->detailText->setText(hash);
}
