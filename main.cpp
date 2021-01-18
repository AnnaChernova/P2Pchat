#include "p2pchat.h"
//#include <QApplication>
#include <QCoreApplication>

#include "server.h"
#include "client.h"
#include "peer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    Server server;

//    Client krisya;
//    krisya.signalEmitator();
//    QApplication a(argc, argv);
//    P2Pchat w;
//    w.show();
    Peer firstPeer;
    return a.exec();
}
