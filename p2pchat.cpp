#include "p2pchat.h"
#include "ui_p2pchat.h"

P2Pchat::P2Pchat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::P2Pchat)
{
    ui->setupUi(this);
}

P2Pchat::~P2Pchat()
{
    delete ui;
}
