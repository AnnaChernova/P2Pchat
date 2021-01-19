#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QTextTableFormat>

#include "peer.h"
#include "ui_chatdialog.h"

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

private slots:
    void userSendMsg();
    void showNewMessage(const QString &from, const QString &message);
    void showNewUser(const QString &nick);
    void showUserLeft(const QString &nick);

private:
    Ui::ChatDialog *ui;
    Peer peer;
    QString myNickName;
    QTextTableFormat tableFormat;
};

#endif // CHATDIALOG_H
