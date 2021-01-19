#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>
#include <QDebug>

#include <QTextStream>
#include <QTime>
#include <QMessageBox>
#include <QDateTime>
#include <QString>

#include <memory>
#include <vector>

class Server : public QTcpServer
{
    Q_OBJECT

    QTcpServer* pServerSocket;
    std::vector<QTcpSocket*> clients;

    const QString peersHistoryFilename = "PeersHistory.txt";
    const QString chatHistoryFilename = "ChatHistory.txt";

public:
    Server(QObject* parent = nullptr);

    QString getServerIP();
    uint getServerPort();
    void sendMsgToClient(QByteArray sendingText);

signals:
    void newMessage(QString sender, QString newMsg);
    void sendFileToClients(QByteArray sendingFile);

private slots:
    void newConnection();
    void clientDisconnection();
    void readMsgFromClient();

    void sendPeersListToClients(QByteArray sendingFile);
    void sendHistoryFileToClients(QByteArray sendingFile);
};

#endif // SERVER_H

