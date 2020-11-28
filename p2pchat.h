#ifndef P2PCHAT_H
#define P2PCHAT_H

#include <QMainWindow>
#include <QString>
#include <QtSql/QSql>
#include "makeserver.h"
#include "makeclient.h"


namespace Ui {
    class P2Pchat;
}

class P2Pchat : public QMainWindow
{
    Q_OBJECT

public:
    explicit P2Pchat(QWidget *parent = nullptr);
    ~P2Pchat();

private slots:
    // Кнопки для выбора режима подключения.
    void on_serverButton_clicked();
    void on_clientButton_clicked();

private:
    Ui::P2Pchat *ui;

    MakeServer *ui_MakeServer;
    MakeClient *ui_MakeClient;

    QString m_username;
    QString m_serverIP;
    QString m_serverPort;
};

#endif // P2PCHAT_H
