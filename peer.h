#ifndef PEER_H
#define PEER_H

#include "server.h"
#include "connection.h"


class Peer : public QObject
{
    Q_OBJECT

    Server* pServer = nullptr;
    Connection* pClient = nullptr;

    const QString peersHistoryFilename = "PeersHistory.txt";
    const QString chatHistoryFilename = "ChatHistory.txt";

    QList<QPair<QString, uint>> Peers; // from PeersHistoryFile
    QList<QString> history;

    bool isPeersHistoryExists();

public:
    Peer(QObject* parent = nullptr);

    void readPeersFromFile();
    void refreshChatHistoryLocal();
    void refreshPeersHistoryFile();

    // Серверная часть от пира.
    void tellHistorytoClients();

    // Клиентская часть от пира.
    void tellPeersListToServer();
    void sendMessageFromUiClientToServer(const QString &message);
    void sendMsgToServer(QByteArray sendingText);

signals:
    void newMessage(const QString &from, const QString &message);
    void userConnected(const QString &nick);
    void userLeft(const QString &nick);

private slots:
    void recordingNewMsgEverywhere(QString sender, QString newMsg);

};

#endif // PEER_H
