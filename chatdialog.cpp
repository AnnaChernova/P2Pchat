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
    connect(&peer, &Peer::newMessage_fromPeer_fromClient_fromServer, this, &ChatDialog::showNewMessage);
    connect(&peer, &Peer::userConnected_fromPeer_fromClient_fromServer, this, &ChatDialog::showNewUser);
    connect(&peer, &Peer::userDisconnected_fromPeer_fromClient_fromServer, this, &ChatDialog::showUserLeft);
    connect(&peer, &Peer::historyForNewUser_fromPeer_fromClient_fromServer, this, &ChatDialog::showHistory);
    connect(&peer, &Peer::showPeers_fromPeer_fromClient_fromServer, this, &ChatDialog::appendPeer);

    tableFormat.setBorder(0);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::userSendMsg()
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "ChatDialog::userSendMsg BEGIN";
    QString text = ui->lineEdit->text();
    if (text.isEmpty()) return;

    if (text.startsWith(QChar('/')))
    {
        QColor color = ui->textEdit->textColor();
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("! Unknown command: %1").arg(text.left(text.indexOf(' '))));
        ui->textEdit->setTextColor(color);
    }
    else
    {
        peer.sendMessage(text);
    }

    ui->lineEdit->clear();
    qDebug() << QTime::currentTime().toString() << "ChatDialog::userSendMsg END";
}

void ChatDialog::showNewMessage(const QString &from, const QString &message)
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "ChatDialog::showNewMessage BEGIN";
    qDebug() << QTime::currentTime().toString() << "FROM" << from << "MESSAGE" << message;
    if (from.isEmpty() || message.isEmpty()) return;

    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(message);
    QScrollBar *bar = ui->textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
    qDebug() << QTime::currentTime().toString() << "ChatDialog::showNewMessage END";
}

void ChatDialog::showNewUser(const QString &nick)
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "ChatDialog::showNewUser" << nick;
    if (nick.isEmpty()) return;

    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::darkGray);
    ui->textEdit->append(tr("* %1 has joined").arg(nick));
    ui->textEdit->setTextColor(color);
    ui->listWidget->addItem(nick);

}

void ChatDialog::appendPeer(const QList<QPair<QString, quint16>>& peers)
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "ChatDialog::appendPeer" << peers;

    QList<QListWidgetItem *> items = ui->listWidget->findItems(".", Qt::MatchContains);
    qDebug() << QTime::currentTime().toString() << "items" << items;
}

void ChatDialog::showUserLeft(const QString &nick)
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "ChatDialog::showUserLeft" << nick;
    if (nick.isEmpty()) return;

    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::darkGray);
    ui->textEdit->append(tr("* %1 has left").arg(nick));
    ui->textEdit->setTextColor(color);

    QList<QListWidgetItem *> items = ui->listWidget->findItems(nick, Qt::MatchExactly);
    if (items.isEmpty()) return;
    qDebug().noquote() << QTime::currentTime().toString() << "items" << items;
    delete items.at(0);
}

void ChatDialog::showHistory(QList<QString> &history)
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "ChatDialog::showHistory BEGIN";
    QString from;
    QString message;
    QList<QString> hocalH = history;
    qDebug() << QTime::currentTime().toString() << "history.length() =" << history.length();

    for (auto &record : hocalH)
    {
        QTextStream tmpStream(&record);

        tmpStream >> from;
        tmpStream.skipWhiteSpace();
        message = tmpStream.readAll();

        showNewMessage(from, message);
    }
    qDebug() << QTime::currentTime().toString() << "ChatDialog::showHistory END";
}
