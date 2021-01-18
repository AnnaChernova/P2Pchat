#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>
#include <QDebug>

#include <QTextStream>
#include <QTime>
#include <QMessageBox>
#include <QDateTime>
#include <QString>

#include <memory>
#include <vector>

class Server : public QTcpServer
{
    Q_OBJECT

    QTcpServer* pServerSocket;
    std::vector<QTcpSocket*> clients;

public:
    explicit Server(QObject* parent = nullptr);

    QString getServerIP();
    uint getServerPort();
    void sendMsgToClient(QByteArray sendingText);

private slots:
    void newConnection();
    void clientDisconnection();
    void readMsgFromClient();
};

#endif // SERVER_H

