#pragma once

#include <QCloseEvent>
#include <QTcpSocket>
#include <QWidget>

namespace Ui {
class Messeger;
}

class Messeger : public QWidget
{
    Q_OBJECT

public:
    Messeger(QTcpSocket *socket_);
    ~Messeger();

public slots:
    void on_lineEdit_returnPressed();

    void on_pushButton_clicked();

    void slotReadyRead();

    void SendToServer(QString str);

private:
    Ui::Messeger *ui;
    QTcpSocket *socket;
    QByteArray Data;
};
