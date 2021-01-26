#include "peer.h"

Peer::Peer()
{
    // Чтение файла настроек.
    QString tmp_port;
    QFile inputFile(configFilename);
    if (inputFile.open(QFile::Text | QFile::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QString text = in.readLine();
            QStringList line = text.split(' ');

            if (line.size() == 2)
            {
                bool ok;
                listeningPort = line.at(1).toUInt(&ok);
                localIP = line.at(0).trimmed();

                if (ok == false)
                {
                    throw "Invalid port in config file!";
                }
            }
            else
            {
                throw "Invalid arguments in config file!";
            }
        }
        inputFile.close();
    }
    qDebug() << "CONFIG " << localIP << listeningPort;

    // Настройка клиента.
    pClient = new Connection();
    pClient->setIP(localIP);
    pClient->setPort(listeningPort);

    connect(pClient, &Connection::connected, this, &Peer::successfullConnection_fromClient);
    connect(pClient, &Connection::userConnected_fromClient_fromServer, this, &Peer::newConnection);
    connect(pClient, &Connection::userDisconnected_fromClient_fromServer, this, &Peer::disconnection);
    connect(pClient, &Connection::newMessage_fromClient_fromServer, this, &Peer::newMessage);
    connect(pClient, &Connection::historyForNewUser_fromClient_fromServer, this, &Peer::gettingHistoryFromServer);
    connect(pClient, &Connection::newListOfPeers_fromClient_fromServer, this, &Peer::gettingNewPeersFromServer);

    // Настройка собственного сервера у пира.
    pPeerServer = new Server(QHostAddress(localIP), listeningPort);
    qDebug() << "P_PEER_SERVER ::" << pPeerServer->serverAddress() << pPeerServer->serverPort();

    // Обновление программного списка известных пиров с файла истории пиров.
    readPeersFromFile();

    // Добавляем себя в качестве доступного пира по config.txt в список, если нас там ещё нет.
    QPair<QString, quint16> ppeer = qMakePair(pPeerServer->serverAddress().toString(), pPeerServer->serverPort());
    if(!knownPeers.contains(ppeer))
    {
        qDebug().noquote() << "\nНовая запись в списке пиров:" << ppeer;
        knownPeers.push_back(ppeer);
        qDebug() << "PEERS" << knownPeers;
        writePeersToFile();
    }

    tryToConnect();

    // Отсылаем серверу информацию о себе.
    QString tellAboutMe = pClient->getIP() + ':' + QString::number(pClient->getPort());
    pClient->sendToServer(convertToNewConnDT(tellAboutMe) + '/');
    qDebug().noquote() << "\n";
    qDebug() << QTime::currentTime().toString() << "Peer::Peer SENT FROM CONSTRUCTOR" << convertToNewConnDT(tellAboutMe);

    // Отсылаем серверу информацию об известных пирах.
    pClient->sendToServer(convertToListOfPeersDT(knownPeersToQString()));
    qDebug().noquote() << "\n";
    qDebug() << QTime::currentTime().toString() << "Peer::Peer SENT FROM CONSTRUCTOR" << convertToListOfPeersDT(knownPeersToQString());
}

void Peer::tryToConnect()
{
    if(!knownPeers.isEmpty())
    {
        // Перебираем адреса, пока не будет подключения.
        for (auto &peer : knownPeers)
        {
            qDebug().noquote() << "\nПытаемся подключиться к известному серверу №" << knownPeers.indexOf(peer);

            pClient->connectToHost(peer.first, peer.second);

            if (pClient->waitForConnected(3000))
            {
                auto nServer = knownPeers.indexOf(peer);
                connect(pClient, &QAbstractSocket::disconnectFromHost , this, &Peer::connectingToServer);
                qDebug() << "Подключены к серверу №" << nServer << knownPeers.at(nServer);
                break;
            }
        }
    }
}

/**
 *  Обновляет список пиров в контейнере путём чтения из файла.
 */
void Peer::readPeersFromFile()
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Peer::readPeersFromFile BEGIN";
    QFile PeersHistoryFile(peersHistoryFilename);
    if (!PeersHistoryFile.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream is(&PeersHistoryFile);
    qDebug() << QTime::currentTime().toString() << peersHistoryFilename << "opened (ReadOnly).";

    while (!is.atEnd())
    {
        QString curLine = is.readLine();
        QString ip_address;
        quint16 port;
        QTextStream parseLine (&curLine);
        parseLine >> ip_address >> port;
        knownPeers.emplace_back(ip_address, port);
    }
    PeersHistoryFile.close();
    qDebug() << QTime::currentTime().toString() << "PEERS" << knownPeers;
    qDebug() << QTime::currentTime().toString() << peersHistoryFilename << "closed (ReadOnly).";
    qDebug() << QTime::currentTime().toString() << "Peer::readPeersFromFile END";
}

/**
 * @brief Склеивает лист известных пиров в строку.
 */
QString Peer::knownPeersToQString()
{
    QString sendingText;
    QTextStream os(&sendingText);

    for (auto &peer : knownPeers)
    {
        os << peer.first << ' ' << peer.second << '\n';
    }

    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Peer::knownPeersToQString" << sendingText;
    return sendingText;
}

QString Peer::clientNickName() const
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Peer::clientNickName" << pClient->getIP() + ":" + QString::number(pClient->getPort());
    return pClient->getIP() + ":" + QString::number(pClient->getPort());
}

/**
 * @brief Вызывает метод отправки сообщения у клиента, который потом напишет в сокет.
 * @param message - отправляемое сообщение
 */
void Peer::sendMessage(const QString &message)
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Peer::sendMessage BEGIN";
    if (message.isEmpty())
        return;

    QString stringMsgWithAdresser = clientNickName() + " " + message;

    pClient->sendToServer(convertToMessageDT(stringMsgWithAdresser));
    qDebug() << QTime::currentTime().toString() << "MESSAGE SENT" << convertToMessageDT(stringMsgWithAdresser);
    qDebug() << QTime::currentTime().toString() << "Peer::sendMessage BEGIN";
}

/**
 * @brief Пробрасывает сигнал от сервера о приходе нового пользователя,
 *        полученный пиром от клиента, дальше, в UI.
 * @param nick - имя пользователя
 */
void Peer::newConnection(const QString &nick)
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Peer::newConnection BEGIN";
    emit userConnected_fromPeer_fromClient_fromServer(nick);

    // Отсылаем серверу информацию об известных пирах.
    pClient->sendToServer(convertToListOfPeersDT(knownPeersToQString()));
    qDebug().noquote() << "\n";
    qDebug() << QTime::currentTime().toString() << "Peer::Peer SENT FROM CONSTRUCTOR" << convertToListOfPeersDT(knownPeersToQString());

    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "emit userConnected_fromPeer_fromClient_fromServer" << nick;
}

/**
 * @brief Пробрасывает сигнал от сервера об уходе пользователя,
 *        полученный пиром от клиента, дальше, в UI.
 * @param nick - имя пользователя
 */
void Peer::disconnection(const QString &nick)
{
    emit userDisconnected_fromPeer_fromClient_fromServer(nick);
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "emit userDisconnected_fromPeer_fromClient_fromServer" << nick;
}

/**
 * @brief Пробрасывает сигнал от клиента о том, что пришло сообщение,
 *        полученный пиром от клиента, дальше, в UI.
 * @param from - от кого пришло сообщение
 * @param message - само сообщение
 */
void Peer::newMessage(const QString& from, const QString& message)
{
    emit newMessage_fromPeer_fromClient_fromServer(from, message);
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "emit newMessage_fromPeer_fromClient_fromServer" << from << message;
}

/**
 * @brief Пробрасывает сигнал от клиента о том, что пришла история,
 *        полученная пиром от клиента, дальше, в UI.
 * @param stringHistory - история в нераспарсенном виде
 */
void Peer::gettingHistoryFromServer(QString& stringHistory)
{
    QList<QString> history;
    QTextStream in(&stringHistory);

    while (!in.atEnd())
    {
        history.append(in.readLine());
    }

    emit historyForNewUser_fromPeer_fromClient_fromServer(history);
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "emit historyForNewUser_fromPeer_fromClient_fromServer" << history;
}

/**
 * @brief Обнаруживает, что клиент подключился к серверу:
 *        ловит сигнал Connection-a,
 *        который по сути является QTcpSocket-ом.
 */
void Peer::successfullConnection_fromClient()
{
    successfullConnection = true;
}

/**
 * @brief Обнаруживает, что клиенту пришёл новый список пиров,
 *        ловит сигнал Connection-a,
 *        который по сути является QTcpSocket-ом.
 */
void Peer::gettingNewPeersFromServer(QString& stringPeers)
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Peer::gettingNewPeersFromServer BEGIN" << "GET" << stringPeers;
    QList<QPair<QString, quint16>> newPeers;
    QTextStream in(&stringPeers);

    while (!in.atEnd())
    {
        QString address;
        quint16 port;

        QString tmpLine(in.readLine());
        QTextStream tmpStream(&tmpLine);

        tmpStream >> address;
        tmpStream.skipWhiteSpace();
        tmpStream >> port;

        newPeers.emplaceBack(address, port);
    }
    knownPeers = newPeers;

    writePeersToFile();
    qDebug() << QTime::currentTime().toString() << "Peer::gettingNewPeersFromServer END";
}

void Peer::connectingToServer()
{
    pClient = new Connection();
    pClient->setIP(localIP);
    pClient->setPort(listeningPort);

    tryToConnect();
}

/**
 *  @brief Перезаписывает данные в файле из контейнера.
 */
void Peer::writePeersToFile()
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Peer::refreshPeersHistoryFile BEGIN";
    QFile PeersHistoryFile(peersHistoryFilename);
    if(!PeersHistoryFile.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream os(&PeersHistoryFile);
    qDebug() << QTime::currentTime().toString() << "File " << peersHistoryFilename << " is open (WriteOnly).";

    for (auto &record : knownPeers)
    {
        os << record.first << " " << record.second << '\n';
    }
    qDebug() << QTime::currentTime().toString() << "PEERS" << knownPeers;
    qDebug() << QTime::currentTime().toString() << "File " << peersHistoryFilename << " is closed (WriteOnly).";
    qDebug() << QTime::currentTime().toString() << "Peer::refreshPeersHistoryFile END";
}
