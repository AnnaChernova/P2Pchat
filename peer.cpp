#include "peer.h"


Peer::Peer(QObject* parent) : QObject(parent)
{
    pServer = new Server();
    pServer->listen(QHostAddress::Any);
    connect(pServer, &Server::newMessage, this, &Peer::recordingNewMsgEverywhere);

    pClient = new Connection();
//    connect(pClient, pClient->readyForUse())

//    QFile fileIP(ipFilename);
//    if(fileIP.exists())
//    {
//        QList<QPair<QString, uint>> destination;

//        if (!fileIP.open(QIODevice::ReadOnly | QIODevice::Text)) return;
//        QTextStream is(&fileIP);

//        while (!is.atEnd())
//        {
//            QString ip_address;
//            uint port;
//            is >> ip_address >> port;
//            Peers.emplace_back(ip_address, port);
//        }
//    }
//    if(isPeersHistoryExists())
//    {

//    }
    QFile fPH(peersHistoryFilename);
    if(fPH.exists() && isPeersHistoryExists())
    {
//         pClient->setServerIP(Peers.at(0).first);
//         pClient->setServerPort(Peers.at(0).second);

//         pClient->connectToServer();

         QByteArray sendingText = "E pur si muove!";
    }
    else //  Сами становимся сервером.
    {
//        pServer = new Server();
//        pClient->setServerIP(pServer->getServerIP());
//        pClient->setServerPort(pServer->getServerPort());

//        pClient->connectToServer();

        // Записываем себя в список, обновляем его в файле и отсылаем серверу обратно.
        refreshPeersHistoryFile();
    }

    // Подключение клиента к серверу.
    tellPeersListToServer();
    tellHistorytoClients();
}

bool Peer::isPeersHistoryExists()
{
    readPeersFromFile();
    if (Peers.isEmpty())
        return false;
    return true;
}

/**
 * Эмитирует сигнал отправки файла серверу
 * и отправляет ему QByteArray, состоящий
 * из известных ему пиров.
 */
void Peer::tellPeersListToServer()
{
    readPeersFromFile();

    QByteArray sendingText;
    QDataStream os(&sendingText, QIODevice::WriteOnly);

    for (auto &peer : Peers)
    {
        os << peer.first << peer.second;
    }

    emit pClient->sendingFileFromFirstConnection(sendingText);

    qDebug() << QTime::currentTime().toString() << "Peer::tellPeersListToServer() File PeersHistoryFile was sended to the server.";
}

void Peer::sendMessageFromUiClientToServer(const QString &message)
{
    if (message.isEmpty()) return;
    pClient->sendMsgToServer(message.toUtf8());
}

/**
 * Эмитирует сигнал отправки файла клиенту
 * и передаёт ему QByteArray, содержащий
 * всю историю чата.
 */
void Peer::tellHistorytoClients()
{
    refreshChatHistoryLocal();

    QByteArray sendingText;
    QTextStream os(&sendingText, QIODevice::WriteOnly);

    for (auto &record : history)
    {
        os << record;
    }

    emit pServer->sendFileToClients(sendingText);

    qDebug() << QTime::currentTime().toString() << "Peer::tellHistorytoClients() File ChatHistoryFile was sended to the client.";
}

/**
 *  Обновляет список пиров в контейнере путём чтения из файла.
 */
void Peer::readPeersFromFile()
{
    QFile PeersHistoryFile(peersHistoryFilename);
    if (!PeersHistoryFile.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream is(&PeersHistoryFile);
    qDebug() << QTime::currentTime().toString() << "Peer::readPeersFromFile()" << "File " << peersHistoryFilename << " is open (ReadOnly).";

    while (!is.atEnd())
    {
        QString ip_address;
        uint port;
        is >> ip_address >> port;
        Peers.emplace_back(ip_address, port);
    }

    qDebug() << QTime::currentTime().toString() << "Peer::readPeersFromFile()" << "File " << peersHistoryFilename << "is closed (ReadOnly).";
}

/**
 * Обновляет историю сообщений в контейнере путём чтения из файла.
 */
void Peer::refreshChatHistoryLocal()
{
    QFile ChatHistoryFile(chatHistoryFilename);
    if (!ChatHistoryFile.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream is(&ChatHistoryFile);
    qDebug() << QTime::currentTime().toString() << "Peer::refreshChatHistoryLocal()" << "File " << chatHistoryFilename << " is open (ReadOnly).";

    QList<QString> tmpHistory;
    while (!is.atEnd())
    {
        QString line = is.readLine();
        tmpHistory.push_back(line);
    }
    history = tmpHistory;

    qDebug() << QTime::currentTime().toString() << "Peer::refreshChatHistoryLocal()" << "File " << chatHistoryFilename << "is closed (ReadOnly).";
}

/**
 *  Добавляет новый пир в контейнер и сразу перезаписывает данные в файле.
 */
void Peer::refreshPeersHistoryFile()
{
    QPair<QString, uint> qpair = qMakePair(pServer->getServerIP(), pServer->getServerPort()); // ? ? ?
    if(!Peers.contains(qpair))
    {
        Peers.push_back(qpair);
    }

    QFile PeersHistoryFile(peersHistoryFilename);
    if(!PeersHistoryFile.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream os(&PeersHistoryFile);
    qDebug() << QTime::currentTime().toString() << "File " << peersHistoryFilename << " is open (WriteOnly).";

    for (auto &record : Peers)
    {
        os << record.first << " " << record.second << '\n';
    }

    qDebug() << QTime::currentTime().toString() << "File " << peersHistoryFilename << " is closed (WriteOnly).";
}


/**
 * Добавление нового сообщения во все места:
 * - в локальную историю (контейнер);
 * - в файл.
 */
void Peer::recordingNewMsgEverywhere(QString sender, QString newMsg)
{
    QString newData = sender + newMsg;
    history.push_back(newData);

    QFile ChatHistoryFile(chatHistoryFilename);
    if(!ChatHistoryFile.open(QIODevice::WriteOnly)) return;

    QByteArray sendingText;
    QTextStream os(&sendingText, QIODevice::WriteOnly);
    qDebug() << QTime::currentTime().toString() << "File " << chatHistoryFilename << " is open (WriteOnly).";

    for(auto &record : history)
    {
        os << record;
    }

    qDebug() << QTime::currentTime().toString() << "File " << chatHistoryFilename << " is closed (WriteOnly).";
}
