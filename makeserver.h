#ifndef MAKESERVER_H
#define MAKESERVER_H

#include <QWidget>

namespace Ui {
class MakeServer;
}

class MakeServer : public QWidget
{
    Q_OBJECT

public:
    explicit MakeServer(QWidget *parent = nullptr);
    ~MakeServer();

signals:
    void MainMenu(); // Сигнал на открытие окна выбора режима подключения.

private slots:
    void on_btn_returnToMenu_clicked();

private:
    Ui::MakeServer *ui;
};

#endif // MAKESERVER_H
