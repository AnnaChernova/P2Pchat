#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

#include <QByteArray>
#include <QTextStream>
#include <QTime>
#include <QThread>

#include "DTP.hpp"

class Server : public QTcpServer
{
    Q_OBJECT

    QList<QTcpSocket*> activeUsers;
    QList<QPair<QString, quint16 >> knownPeers;
    QList<QString> history;
    QMap<QTcpSocket*, QString> socket_hisPeer;

public:
    Server(const QHostAddress &address = QHostAddress::Any, quint16 port = 0, QObject *parent = nullptr);
    ~Server();

signals:
    void newMessage(const QString& from, const QString& message);

private slots:
    void newConnection();
    void disconnection();

private:
    void readMessage();
    void serverBroadcast(const QString& message);
    void parseListOfPeers(QString& listOfPeers);
    QString packHistory(QList<QString>& history);
    QString packListOfPeers(QList<QPair<QString, quint16>>& listOfPeers);
};

#endif // SERVER_H
