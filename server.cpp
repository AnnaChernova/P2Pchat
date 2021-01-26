#include "server.h"

Server::Server(const QHostAddress &address, quint16 port, QObject *parent)
    : QTcpServer(parent)
{
    listen(address, port);
    connect(this, &QTcpServer::newConnection, this, &Server::newConnection);
}

Server::~Server()
{
    for (auto& connectoin : activeUsers)
    {
        connectoin->close();
    }
}

void Server::newConnection()
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Server::newConnection BEGIN";
    QTcpSocket* newUser = nextPendingConnection();

    // Добавляем его в список активных соединений.
    activeUsers.append(newUser);
    qDebug() << QTime::currentTime().toString() << "Server::newConnection New User" << newUser->peerAddress().toString() << newUser->peerPort();

    connect(newUser, &QTcpSocket::readyRead, this, &Server::readMessage);
    connect(newUser, &QTcpSocket::disconnected, this, &Server::disconnection);

    // Отсылаем историю новому участнику.
    QString historyMsg = convertToHistoryDT(packHistory(this->history));// + '/';
    newUser->write(historyMsg.toUtf8());
    qDebug() << QTime::currentTime().toString() << "HISTORY SENT TO NEW USER!" << historyMsg;
    newUser->flush();

    qDebug() << QTime::currentTime().toString() << "Server::newConnection END";
}

void Server::parseListOfPeers(QString& listOfPeers)
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Server::parseListOfPeers BEGIN";
    QTextStream streamRaw(&listOfPeers);

    while(!streamRaw.atEnd())
    {
        QString curLine = streamRaw.readLine();
        QTextStream parseLine(&curLine);
        QString peerIP;
        quint16 peerPort;
        parseLine >> peerIP >> peerPort;

        QPair<QString, quint16> newpeer = qMakePair(peerIP, peerPort);
        if (!knownPeers.contains(newpeer))
        {
            knownPeers.append(newpeer);
        }
    }
    qDebug() << QTime::currentTime().toString() << "LIST_SIZE" << knownPeers;
    qDebug() << QTime::currentTime().toString() << "Server::parseListOfPeers END";
}

QString Server::packHistory(QList<QString>& history)
{
    QString newMsg;
    QTextStream streamMsg(&newMsg);
    for(auto& record : history)
    {
        streamMsg << record << '\n';
    }
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Server::packHistory HISTORY" << newMsg;
    return newMsg;
}

QString Server::packListOfPeers(QList<QPair<QString, quint16>>& listOfPeers)
{
    QString newMsg;
    QTextStream streamMsg(&newMsg);
    for(auto& peer : listOfPeers)
    {
        streamMsg << peer.first << ' ' << peer.second << '\n';
    }
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Server::packListOfPeers PEERS" << newMsg;
    return newMsg;
}

void Server::disconnection()
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Server::disconnection BEGIN";
    QTcpSocket* disUser = qobject_cast<QTcpSocket*>(sender());

    // Отсылаем всем информацию об ушедшем участнике.
    auto nick = socket_hisPeer.find(disUser);
    serverBroadcast(convertToDisconnDT(*nick));//+ '/');

    // Удаляем его из списка активных соединений.
    qDebug() << QTime::currentTime().toString() << "DISCONNECTED CLIENT nick" << disUser << *nick;
    qDebug() << QTime::currentTime().toString() << "ACTIVE USERS" << activeUsers;
    activeUsers.removeAll(disUser);
    socket_hisPeer.remove(disUser);
    qDebug() << QTime::currentTime().toString() << "ACTIVE USERS" << activeUsers;
    qDebug() << QTime::currentTime().toString() << "Server::disconnection END";
}

void Server::readMessage()
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Server::readMessage BEGIN";
    QTcpSocket* senderCon = qobject_cast<QTcpSocket*>(sender());
    QByteArray rawMessage = senderCon->readAll();
    qDebug() << QTime::currentTime().toString() << "NEW_MESSAGE_IN_SERVER" << rawMessage;

    QString dataAsString = QString(rawMessage);
    QList<QString> list = dataAsString.split('/');
    qDebug() << QTime::currentTime().toString() << "list.size()=" << list.size();
    for (auto &message : list)
    {
        QString msgWithoutType = getDataWithoutType(QString(message));

        qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Server::readMessage";

        DataType dataType = whatKindOfData(QString(message));

        switch (dataType)
        { // по логике сюда не может прийти ничего, кроме Message, KnownPeers и NewConnection
            case DataType::Message:
            {
                history.push_back(msgWithoutType);

                serverBroadcast(convertToMessageDT(msgWithoutType));
                qDebug() << QTime::currentTime().toString() << "DataType::Message SENT" << convertToMessageDT(msgWithoutType);
                qDebug() << QTime::currentTime().toString() << "Server::readMessage END";
                return;
            }
            case DataType::KnownPeers:
            {
                parseListOfPeers(msgWithoutType);

                QString newKnownPeersMsg = packListOfPeers(knownPeers);
                qDebug() << QTime::currentTime().toString() << "DataType::KnownPeers SENT" << convertToListOfPeersDT(newKnownPeersMsg);
                qDebug() << QTime::currentTime().toString() << "Server::readMessage END";
                return;
            }
            case DataType::NewConnection:
            {
                // msgWithoutType в сообщении является ником
                socket_hisPeer.insert(senderCon, msgWithoutType);
                qDebug() << QTime::currentTime().toString() << "socket_hisPeer" << socket_hisPeer;

                // Добавляем в список известных пиров.
                QList<QString> list = msgWithoutType.split(':');
                QString new_ip = list.at(0);
                quint16 new_port = list.at(1).toUInt();
                knownPeers.emplaceBack(new_ip,new_port);
                qDebug() << QTime::currentTime().toString() << "knownPeers" << knownPeers;

                // Рассылаем всем информацию о новом подключении.
                serverBroadcast(message);
                qDebug() << QTime::currentTime().toString() << "DataType::NewConnection SENT" << message;
                qDebug() << QTime::currentTime().toString() << "Server::readMessage END";
                return;
            }
            case DataType::Disconnection:
            case DataType::History:
            case DataType::Unknown:
            {
                qDebug() << QTime::currentTime().toString() << "THAT MESSAGE SHOULD NOT BE SEEN (something really bad happened --> a bad message came to the server)";
                qDebug() << QTime::currentTime().toString() << "Server::readMessage END";
                return;
            }
        }
    }
}

/**
 * @brief ОБЯЗАТЕЛЬНО СКОНВЕРТИРОВАТЬ ИНФОРМАЦИЮ В НЕОБХОДИМЫЙ ВИД!
 * @param message - информация для всех пользователей
 * Информация может быть любого поддерживаемого протоколом типа.
 */
void Server::serverBroadcast(const QString &message)
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Server::serverBroadcast BEGIN";
    QByteArray data = message.toUtf8();
    qDebug() << QTime::currentTime().toString() << "MESSAGE" << message;
    qDebug() << QTime::currentTime().toString() << "ACTIVE USERS" << activeUsers.length();

    for (auto user : activeUsers)
    {
        user->write(data);
    }
    qDebug() << QTime::currentTime().toString() << "Server::serverBroadcast END";
}
