#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QtNetwork>
#include <QDebug>

#include <QString>

class Client : public QObject
{
    Q_OBJECT

    QTcpSocket* pClientSocket;
    QString serverIP;
    uint serverPort;

public:
    Client(QObject* parent = nullptr);

    void firstConnectionToServer();

    QString getClientIP();
    uint getClientPort();

    void setServerIP(QString sIP);
    void setServerPort(uint sPort);

signals:
    void valueChanged(QByteArray sendingText);

private slots:
    void readMsgFromServer();
    void sendMsgToServer(QByteArray sendingText);
};

#endif // CLIENT_H

