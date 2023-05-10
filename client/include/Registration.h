#pragma once

#include <QTcpSocket>
#include <QWidget>

namespace Ui {
class Registration;
}

class Registration : public QWidget
{
    Q_OBJECT

public:
    explicit Registration(QTcpSocket *socket_);
    ~Registration();

public slots:
    void slotReadyRead();

    void sendToServer(QString login, QString password);

signals:
    void signalRegistration(QString login, QString password);

private slots:
    void on_pushButton_pressed();

private:
    Ui::Registration *ui;
    QTcpSocket *socket;
    QByteArray Data;
};

