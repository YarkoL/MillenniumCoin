#ifndef TRANSACTIONDESCDIALOG_H
#define TRANSACTIONDESCDIALOG_H

//http://www.informit.com/articles/article.aspx?p=1405224
#include <QDialog>

namespace Ui {
    class TransactionDescDialog;
}
QT_BEGIN_NAMESPACE
class QModelIndex;
class QPushButton;
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

signals:
   // void retrieve(const QString &str);

private slots:
    void retrieveClicked();
   // void enableRetrieveButton(const QString &txt);
};

#endif // TRANSACTIONDESCDIALOG_H
