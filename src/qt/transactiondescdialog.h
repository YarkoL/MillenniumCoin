#ifndef TRANSACTIONDESCDIALOG_H
#define TRANSACTIONDESCDIALOG_H

#include <QDialog>
#include "transactionrecord.h"

namespace Ui {
    class TransactionDescDialog;
}
QT_BEGIN_NAMESPACE
class QModelIndex;

QT_END_NAMESPACE

/** Dialog showing transaction details. */
class TransactionDescDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionDescDialog(const QModelIndex &idx, QWidget *parent = 0);
    ~TransactionDescDialog();


private:
    Ui::TransactionDescDialog *ui;
    uint256 tx_id;
    bool isEscrow;
    int depth;

signals:

public slots:
    void retrieveTxHandler();
};

#endif // TRANSACTIONDESCDIALOG_H
