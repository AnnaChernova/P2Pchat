#include "server.h"

Server::Server(QObject* parent)
    : QTcpServer(parent)
{
    pServerSocket = new QTcpServer(this);

    if(!pServerSocket->listen(QHostAddress::Any, 9999))
    {
        qDebug() << QTime::currentTime().toString() << "Server did not start!";
        QMessageBox::critical(0, "Server Error", "Unable to start the server:" + pServerSocket->errorString());
        pServerSocket->close();
    }
    else
    {
        connect(pServerSocket, &QTcpServer::newConnection, this, &Server::newConnection);
        qDebug() << QTime::currentTime().toString()<< "Server started!" << getServerIP();
    }
}

QString Server::getServerIP()
{
    return QNetworkInterface::allAddresses()[QHostAddress::LocalHost].toString();
}

uint Server::getServerPort()
{
    return pServerSocket->serverPort();
}

void Server::newConnection()
{
//    std::unique_ptr<QTcpSocket> pSocket = std::make_unique<QTcpSocket>(pServer->nextPendingConnection());
    QTcpSocket* pSocket = pServerSocket->nextPendingConnection();
    clients.push_back(pSocket);
    qDebug() << QTime::currentTime().toString() << "New client has just connected." << pSocket->peerAddress().toString();

    connect(pSocket, &QTcpSocket::readyRead, this, &Server::readMsgFromClient);
    connect(pSocket, &QTcpSocket::disconnected, this, &Server::clientDisconnection);

    pSocket->write("Welcome, my friend!\n");
    pSocket->flush();

    pSocket->waitForBytesWritten();
}

void Server::clientDisconnection()
{
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(QObject::sender());
    qDebug() << QTime::currentTime().toString() << "Client disconnected!" << senderSocket->peerAddress().toString();
    senderSocket->close();
}

void Server::readMsgFromClient()
{
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(QObject::sender());
    QByteArray byteArray = senderSocket->readAll();
    qDebug() << QTime::currentTime().toString() << senderSocket->peerAddress().toString() << byteArray;
    sendMsgToClient(byteArray);
}

void Server::sendMsgToClient(QByteArray sendingText)
{
    QByteArray byteArray = "Server>>" + sendingText;
    for (auto peer : clients)
    {
        peer->write(byteArray);
    }
}
