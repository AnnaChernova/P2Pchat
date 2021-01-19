#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QtNetwork>
#include <QDebug>

#include <QString>

class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    Connection(QObject* parent = nullptr);

    void connectToServer(QString serverIP, uint serverPort);

signals:
    void incomingFile();
    void valueChanged(QByteArray sendingText); // сигнал для отправки текста из гуи
    void sendingFileFromFirstConnection(QByteArray sendingFile);

    void readyForUse();
    void newMessage(const QString &from, const QString &message);

public slots:
    void readMsgFromServer();
    void sendMsgToServer(QByteArray sendingText);

    void readFileFromServer();
    void sendFileToServer(QByteArray sendingFile);
};

#endif // CONNECTION_H

