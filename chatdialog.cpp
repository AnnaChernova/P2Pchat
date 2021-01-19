#include <QtWidgets>

#include "chatdialog.h"

ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::ChatDialog)
{
    ui->setupUi(this);

    ui->lineEdit->setFocusPolicy(Qt::StrongFocus);
    ui->textEdit->setFocusPolicy(Qt::NoFocus);
    ui->textEdit->setReadOnly(true);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);

    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &ChatDialog::userSendMsg);
    connect(&peer, &Peer::newMessage, this, &ChatDialog::showNewMessage);
    connect(&peer, &Peer::userConnected, this, &ChatDialog::showNewUser);
    connect(&peer, &Peer::userLeft, this, &ChatDialog::showUserLeft);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::userSendMsg()
{
    QString text = ui->lineEdit->text();
    if (text.isEmpty())
        return;

    if (text.startsWith(QChar('/')))
    {
        QColor color = ui->textEdit->textColor();
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("! Unknown command: %1").arg(text.left(text.indexOf(' '))));
        ui->textEdit->setTextColor(color);
    }
    else
    {
        peer.sendMessageFromUiClientToServer(text);
        showNewMessage(myNickName, text);
    }

    ui->lineEdit->clear();
}

void ChatDialog::showNewMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty()) return;

    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(message);
    QScrollBar *bar = ui->textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void ChatDialog::showNewUser(const QString &nick)
{
    if (nick.isEmpty()) return;

    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->append(tr("* %1 has joined").arg(nick));
    ui->textEdit->setTextColor(color);
    ui->listWidget->addItem(nick);
}

void ChatDialog::showUserLeft(const QString &nick)
{
    if (nick.isEmpty()) return;

    QList<QListWidgetItem *> items = ui->listWidget->findItems(nick, Qt::MatchExactly);
    if (items.isEmpty()) return;

    delete items.at(0);
    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->append(tr("* %1 has left").arg(nick));
    ui->textEdit->setTextColor(color);
}

