#include "connection.h"


Connection::Connection(QObject* parent) : QTcpSocket(parent)
{
    connect(this, &QTcpSocket::readyRead, this, &Connection::readFromServer);
}

/**
 * @brief Отправка чего угодно на сервер. sendingData необходимо
 *        предварительно сконвертировать через ППД.
 * @param sendingData - отправляемый блок данных
 */
void Connection::sendToServer(const QString& sendingDatanew)
{
    QChar endTermonator = '/';
    QString sendingData = sendingDatanew + endTermonator;

    QByteArray byteArray = sendingData.toUtf8();
    write(byteArray);
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Connection::sendToServer SENT TO SERVER" << sendingData;
}

/**
 * @brief Слот обработки всего приходящего на клиент (==клиентский сокет).
 */
void Connection::readFromServer()
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "Connection::readFromServer BEGIN";
    QByteArray rawMessage = this->readAll();
    qDebug() << QTime::currentTime().toString() << "rawMessage" << rawMessage;

    DataType dataType = whatKindOfData(QString(rawMessage));

    QString clearStringMsg = getDataWithoutType(QString(rawMessage));
    qDebug() << QTime::currentTime().toString() << "clearStringMsg " << clearStringMsg;

    switch(dataType)
    {
        case DataType::Message:
        {
            QString from = clearStringMsg.section(" ", 0, 0);
            QString what = clearStringMsg.remove(0, from.length()+1);

            qDebug() << QTime::currentTime().toString() << "DataType::Message";
            qDebug() << QTime::currentTime().toString() << "FROM" << from << "MESSAGE" << what;
            emit newMessage_fromClient_fromServer(from, what);
            qDebug() << QTime::currentTime().toString() << "Connection::readFromServer END";
            return;
        }
        case DataType::History:
        {
            qDebug() << QTime::currentTime().toString() << "DataType::History";
            emit historyForNewUser_fromClient_fromServer(clearStringMsg);
            qDebug() << QTime::currentTime().toString() << "Connection::readFromServer END";
            return;
        }
        case DataType::KnownPeers:
        {
            qDebug() << QTime::currentTime().toString() << "DataType::KnownPeers";
            emit newListOfPeers_fromClient_fromServer(clearStringMsg);
            qDebug() << QTime::currentTime().toString() << "Connection::readFromServer END";
            return;
        }
        case DataType::NewConnection:
        {
            qDebug() << QTime::currentTime().toString() << "DataType::NewConnection";
            emit userConnected_fromClient_fromServer(clearStringMsg);
            qDebug() << QTime::currentTime().toString() << "Connection::readFromServer END";
            return;
        }
        case DataType::Disconnection:
        {
            qDebug() << QTime::currentTime().toString() << "DataType::Disconnection";
            emit userDisconnected_fromClient_fromServer(clearStringMsg);
            qDebug() << QTime::currentTime().toString() << "Connection::readFromServer END";
            return;
        }
        case DataType::Unknown:
            return;
    }
}
