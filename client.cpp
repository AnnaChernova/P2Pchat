#include "client.h"

Client::Client(QObject* parent) : QObject(parent)
{
    pClientSocket = new QTcpSocket();
}

void Client::firstConnectionToServer()
{
    pClientSocket->connectToHost(serverIP,serverPort);
    qDebug() << QTime::currentTime().toString()<< "Client started!" << getClientIP();

    connect(this, &Client::valueChanged, this, &Client::sendMsgToServer);
    connect(pClientSocket, &QTcpSocket::readyRead, this, &Client::readMsgFromServer);
}

QString Client::getClientIP()
{
    return QNetworkInterface::allAddresses()[QHostAddress::LocalHost].toString();
}

uint Client::getClientPort()
{
    return pClientSocket->peerPort();
}

void Client::setServerIP(QString sIP)
{ serverIP = sIP; }

void Client::setServerPort(uint sPort)
{ serverPort = sPort; }

void Client::sendMsgToServer(QByteArray sendingText)
{
    QByteArray byteArray = sendingText;
    pClientSocket->write(byteArray);
}

void Client::readMsgFromServer()
{
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(QObject::sender());
    QByteArray byteArray = senderSocket->readAll();
    qDebug() << QTime::currentTime().toString() << senderSocket->peerAddress().toString() << byteArray;
}
