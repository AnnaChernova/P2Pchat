#include "makeserver.h"
#include "ui_makeserver.h"
#include "ui_p2pchat.h"

MakeServer::MakeServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MakeServer)
{
    ui->setupUi(this);
    MakeServer::setFixedSize(750, 460);
}

MakeServer::~MakeServer()
{
    delete ui;
}

void MakeServer::on_btn_returnToMenu_clicked()
{
    this->close();
    emit MainMenu();
}
