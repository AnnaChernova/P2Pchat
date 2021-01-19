#include <QApplication>

#include "server.h"
#include "connection.h"
#include "peer.h"

#include "chatdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ChatDialog dialog;
    dialog.show();

    return a.exec();
}
