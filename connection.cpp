#include "connection.h"

Connection::Connection(QObject* parent) : QTcpSocket(parent)
{
}

void Connection::connectToServer(QString serverIP,uint serverPort)
{
    connectToHost(serverIP,serverPort);

    connect(this, &Connection::valueChanged, this, &Connection::sendMsgToServer);
    connect(this, &Connection::incomingFile, this, &Connection::readFileFromServer);
    connect(this, &Connection::sendingFileFromFirstConnection, this, &Connection::sendFileToServer);
    connect(this, &QTcpSocket::readyRead, this, &Connection::readMsgFromServer);
}

void Connection::readMsgFromServer()
{
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(QObject::sender());
    QByteArray byteArray = senderSocket->readAll();
    qDebug() << QTime::currentTime().toString() << senderSocket->peerAddress().toString() << byteArray;
}
void Connection::sendMsgToServer(QByteArray sendingText)
{
    QByteArray byteArray = sendingText;
    write(byteArray);
}

/**
 * Принимает файл от сервера и записывает(либо обновляет)
 * его на стороне клиента.
 */
void Connection::readFileFromServer()
{
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(QObject::sender());
    QByteArray byteArray = senderSocket->readAll();
    qDebug() << QTime::currentTime().toString() << senderSocket->peerAddress().toString() << byteArray;
}

void Connection::sendFileToServer(QByteArray sendingFile)
{
    QByteArray byteArray = sendingFile;
    write(byteArray);
}


