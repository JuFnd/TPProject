#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QShortcut>
#include <QDesktopWidget>
#include <QApplication>
#include "bluetoothformatimage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void setImg();
    BluetoothFormatImage *getImg(){return img;};
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    void captureScreen();
    Ui::MainWindow *ui;
    BluetoothFormatImage *img;
};
#endif // MAINWINDOW_H
