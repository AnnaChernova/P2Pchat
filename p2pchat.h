#ifndef P2PCHAT_H
#define P2PCHAT_H

#include <QMainWindow>

namespace Ui {
class P2Pchat;
}

class P2Pchat : public QMainWindow
{
    Q_OBJECT

public:
    explicit P2Pchat(QWidget *parent = nullptr);
    ~P2Pchat();

private:
    Ui::P2Pchat *ui;
};

#endif // P2PCHAT_H
