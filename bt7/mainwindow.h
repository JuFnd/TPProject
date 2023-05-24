#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QShortcut>
#include <QDesktopWidget>
#include <QtGui/QScreen>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QLabel>
#include <QApplication>
#include <QBluetoothServer>
#include <QBluetoothAddress>
#include <QBluetoothLocalDevice>

#include "bluetoothformatimage.h"
#include "client.h"
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    BluetoothFormatImage* getImg(){return img;};
    void setImg();
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendMessage(const QByteArray &data);

private slots:
    void on_sendButton_clicked();

private slots:
    void connectClicked();
    void sendClicked();

    void showMessage(const QString &sender, const QByteArray &message);

    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);
    void clientDisconnected();
    void connected(const QString &name);
    void reactOnSocketError(const QString &error);

    void newAdapterSelected();

private:
    int adapterFromUserSelection() const;
    void captureScreen();
    int currentAdapterIndex = 0;

    Server *server;
    QList<Client *> clients;
    QList<QBluetoothHostInfo> localAdapters;

    QString localName;
    Ui::MainWindow *ui;
    BluetoothFormatImage *img = nullptr;
};
#endif // MAINWINDOW_H
