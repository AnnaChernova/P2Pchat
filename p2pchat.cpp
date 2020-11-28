#include "p2pchat.h"
#include "ui_p2pchat.h"

P2Pchat::P2Pchat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::P2Pchat)
{
    ui->setupUi(this);

    // Инициализация вспомогательных окон.
    ui_MakeServer = new MakeServer();
    ui_MakeClient = new MakeClient();

    // Подключение к слоту запуска главного окна по кнопке во вспомогательных окнах.
    connect(ui_MakeServer, &MakeServer::MainMenu, this, &P2Pchat::show);
    connect(ui_MakeClient, &MakeClient::MainMenu, this, &P2Pchat::show);
}

P2Pchat::~P2Pchat()
{
    delete ui;
}

void P2Pchat::on_serverButton_clicked()
{
    ui_MakeServer->show();
    this->close();
}

void P2Pchat::on_clientButton_clicked()
{
    ui_MakeClient->show();
    this->close();
}
