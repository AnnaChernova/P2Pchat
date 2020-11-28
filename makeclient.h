#ifndef MAKECLIENT_H
#define MAKECLIENT_H

#include <QWidget>

namespace Ui {
class MakeClient;
}

class MakeClient : public QWidget
{
    Q_OBJECT

public:
    explicit MakeClient(QWidget *parent = nullptr);
    ~MakeClient();

    QString getUserName();
    QString getSeverIP();
    QString getServerPort();

signals:
    void MainMenu(); // Сигнал на открытие окна выбора режима подключения.

private slots:
    void on_input_name_textEdited(const QString &arg1);
    void on_input_serverAddres_textEdited(const QString &arg1);
    void on_input_serverPort_textEdited(const QString &arg1);

    void on_btn_returnToMenu_clicked();
    void on_btn_connectToServer_clicked();

private:
    Ui::MakeClient *ui;

    QString m_username;
    QString m_serverIP;
    QString m_serverPort;
};

#endif // MAKECLIENT_H
