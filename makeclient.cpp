#include "makeclient.h"
#include "ui_makeclient.h"

#include "p2pchat.h"

MakeClient::MakeClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MakeClient)
{
    ui->setupUi(this);
    MakeClient::setFixedSize(790, 520);
//    ui->btn_returnToMenu->setStyleSheet("border-radius: 5px;");
}

MakeClient::~MakeClient()
{
    delete ui;
}

/* Геттеры возвращают пользовательский ввод из полей класса.
*/

QString MakeClient::getUserName()
{
    return MakeClient::m_username;
}

QString MakeClient::getSeverIP()
{
    return MakeClient::m_serverIP;
}

QString MakeClient::getServerPort()
{
    return MakeClient::m_serverPort;
}

/* Слоты полей коируют пользовательский ввод в поля класса.
*/

void MakeClient::on_input_name_textEdited(const QString &arg1)
{
    MakeClient::m_username = arg1;
}

void MakeClient::on_input_serverAddres_textEdited(const QString &arg1)
{
    MakeClient::m_serverIP = arg1;
}

void MakeClient::on_input_serverPort_textEdited(const QString &arg1)
{
    MakeClient::m_serverPort = arg1;
}

/* Кнопки пользовательских действий.
 */

void MakeClient::on_btn_returnToMenu_clicked()
{
    this->close();
    emit MainMenu();
}

void MakeClient::on_btn_connectToServer_clicked()
{

}
