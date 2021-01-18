#ifndef PEER_H
#define PEER_H

#include <QFile>
#include <QString>
#include <QSet>
#include <QPair>
#include <QTextStream>
#include <QStringList>

#include <memory>
#include <fstream>

#include "server.h"
#include "client.h"

class Peer
{
    Server* pServer = nullptr;
    Client* pClient = nullptr;

    const QString peersHistoryFilename = "PeersHistory.txt";
    QString chatHistoryFilename = "ChatHistory.txt";

    QFile PeersHistoryFile;
    QFile ChatHistoryFile;

    QSet<QPair<QString, uint>> Peers;

    bool willIPbeEntered = false;
    bool getServerfromPeersHistoryFile = false;

public:
    Peer();

    /*
     * Серверная часть от пира.
     */
    void tellHistorytoClient();
    void getListOfPeersFromClient(std::fstream& pPeershf);
    void parsePeersHistoryFile();
    void refreshPeersHistoryFile();

    /*
     * Клиентская часть от пира.
     */
    std::fstream& tellPeersListToServer();
    void getHistoryFromServer();

private:


};

#endif // PEER_H
