#ifndef DTP_H
#define DTP_H

#include <QString>
#include <QTextStream>
#include <QIODevice>
#include <QDebug>
#include <QTime>

enum class DataType : uint8_t
{
    Unknown = 0,
    Message = 1,
    History = 2,
    KnownPeers = 3,
    NewConnection = 4,
    Disconnection = 5
};

inline DataType whatKindOfData(QString unknownData)
{
    QTextStream stream(&unknownData);

    QString firstSymbol;
    stream >> firstSymbol;

    if (firstSymbol == "1")
        return DataType::Message;
    else if (firstSymbol == "2")
        return DataType::History;
    else if (firstSymbol == "3")
        return DataType::KnownPeers;
    else if (firstSymbol == "4")
        return DataType::NewConnection;
    else if (firstSymbol == "5")
        return DataType::Disconnection;

    return DataType::Unknown;
}

inline QString convertToMessageDT(const QString& message)
{
    return "1 " + message;
}

inline QString convertToHistoryDT(const QString& history)
{
    return "2 " + history;
}

inline QString convertToListOfPeersDT(const QString& peersList)
{
    return "3 " + peersList;
}

inline QString convertToNewConnDT(const QString& nick)
{
    return "4 " + nick;
}

inline QString convertToDisconnDT(const QString& nick)
{
    return "5 " + nick;
}

inline QString getDataWithoutType(QString someData)
{
    qDebug().noquote() << "\n" << QTime::currentTime().toString() << "getDataWithoutType GET" << someData;

    QTextStream stream(&someData);

    QString firstSymbol;
    stream >> firstSymbol;
    stream.skipWhiteSpace();

    QString res = stream.readAll();
    qDebug() << QTime::currentTime().toString() << "getDataWithoutType RET" << res;
    return res;
}

#endif // DTP_H
