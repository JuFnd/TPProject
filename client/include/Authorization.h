#pragma once

#include <QTcpSocket>
#include <QWidget>

namespace Ui {
class Authorization;
}

class Authorization : public QWidget
{
    Q_OBJECT

public:
    explicit Authorization(QTcpSocket *socket);
    ~Authorization();

    void on_pushButton_clicked();

public slots:
    void on_pushButton_pressed();

    void slotReadyRead();

    void sendToServer(QString login, QString password);

    void on_pushButton_2_clicked();

signals:
    void signalAuthTrue();

    void signalRegistration();

private:
    Ui::Authorization *ui;
    QTcpSocket *socket;
    QByteArray Data;
};
