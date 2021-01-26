#ifndef PEER_H
#define PEER_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
#include <QThread>

#include "server.h"
#include "connection.h"

class Peer : public QObject
{
    Q_OBJECT

    Server* pPeerServer;
    Server server;
    Connection* pClient;
    QList<QPair<QString, quint16>> knownPeers;
    quint16 listeningPort;
    QString localIP;

public:
    Peer();

    bool successfullConnection = false;

    const QString peersHistoryFilename = "PeersHistory.txt";
    const QString configFilename = "config.txt";

    void tryToConnect();
    void readPeersFromFile();
    QString knownPeersToQString();
    QString clientNickName() const;
    void sendMessage(const QString &message);

    void refreshChatHistoryLocal();
    void writePeersToFile();

    void unpackNewListOfPeers();

signals:
    void newMessage_fromPeer_fromClient_fromServer(const QString& from, const QString& message);
    void userConnected_fromPeer_fromClient_fromServer(const QString& nick);
    void userDisconnected_fromPeer_fromClient_fromServer(const QString& nick);
    void historyForNewUser_fromPeer_fromClient_fromServer(QList<QString>& history);
    void showPeers_fromPeer_fromClient_fromServer(const QList<QPair<QString, quint16>>& peers);

private slots:
    // С передачей до UI.
    void newConnection(const QString& nick);                        // от клиента
    void disconnection(const QString& nick);                        // от клиента
    void newMessage(const QString& from, const QString& message);   // от клиента
    void gettingHistoryFromServer(QString& stringHistory);          // от клиента

    // Заполнение внутренних полей пира.
    void successfullConnection_fromClient();                        // от клиента
    void gettingNewPeersFromServer(QString& stringPeers);           // от клиента

    void connectingToServer();
};

#endif // PEER_H
