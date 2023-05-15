#ifndef SCREENSHOTS_H
#define SCREENSHOTS_H

#include <QDialog>
#include <QBluetoothServer>
#include <QBluetoothAddress>
#include <QBluetoothLocalDevice>

namespace Ui {
class Screenshots;
}

class Screenshots : public QDialog
{
    Q_OBJECT

public:
    explicit Screenshots(QWidget *parent = nullptr);
    ~Screenshots();

private slots:
    void on_pushButton_clicked();
    void handleConnection();

private:
    void acceptMessage();

    QBluetoothServer* server;
    Ui::Screenshots *ui;
};

#endif // SCREENSHOTS_H
