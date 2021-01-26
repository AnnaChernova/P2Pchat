#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QtNetwork>

#include "DTP.hpp"

class Connection : public QTcpSocket
{
    Q_OBJECT

    QString configIP;
    quint16 configPort;

public:
    Connection(QObject* parent = nullptr);

    QString getIP() const {return configIP;};
    quint16 getPort() const {return configPort;};
    void setIP(const QString& ip) {configIP=ip;}
    void setPort(quint16& port) {configPort=port;}
    void sendToServer(const QString& sendingData);

signals:
    void userConnected_fromClient_fromServer(const QString& nick);
    void userDisconnected_fromClient_fromServer(const QString& nick);
    void newMessage_fromClient_fromServer(const QString& from, const QString& message);
    void newListOfPeers_fromClient_fromServer(QString& stringPeers);
    void historyForNewUser_fromClient_fromServer(QString& stringHistory);

public slots:
    void readFromServer();
};

#endif // CONNECTION_H
