#include "peer.h"

Peer::Peer()
{
    // JUST FOR TEST1.
    pServer = new Server();

    pClient = new Client();

    pClient->setServerIP(pServer->getServerIP());
    pClient->setServerPort(pServer->getServerPort());

    pClient->firstConnectionToServer();

//    pClient->signalEmitator();
    QByteArray sendingText = "E pur si muove!";
    emit pClient->valueChanged(sendingText);

//    pClient = new Client();

//    PeersHistoryFile.rename(peersHistoryFilename);

//    // Выбор сервера.
//    if(willIPbeEntered)
//    {
//        // TODO can't reach to server
//    }
//    else if(getServerfromPeersHistoryFile)
//    {

//    }
//    else //  Сами становимся сервером.
//    {
//        pServer = new Server();


//        // Записываем себя в список и обновляем его в файле.
//        refreshPeersHistoryFile();

//    }

//    // Подключение клиента к серверу.


//    tellPeersListToServer();
//    tellHistorytoClient();

}

//std::fstream& Peer::tellPeersListToServer()
//{
//    // send bytes to server
//    return PeersHistoryFile;
//}

void Peer::tellHistorytoClient()
{
    // send bytes to client
}

void Peer::getListOfPeersFromClient(std::fstream& pPeershf)
{
    // get bytes from client
}

void Peer::parsePeersHistoryFile()
{
    if (!PeersHistoryFile.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&PeersHistoryFile);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        line.chop(1);
        QStringList pair = line.split(' ');
        bool ok;
        QPair<QString, uint> qpair = qMakePair(pair.at(0), pair.at(1).toUInt(&ok, 10));
        Peers.insert(qpair);
    }
}

void Peer::refreshPeersHistoryFile()
{
    QPair<QString, uint> qpair = qMakePair(pServer->getServerIP(), pServer->getServerPort());
    Peers.insert(qpair);  // QQQQ

    if(!PeersHistoryFile.open(QIODevice::WriteOnly)) return;

}

void Peer::getHistoryFromServer()
{
    // get bytes from server
}
