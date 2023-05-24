#ifndef BLUETOOTHFORMATIMAGE_H
#define BLUETOOTHFORMATIMAGE_H

#include <QPixmap>
#include <QApplication>
#include <QBuffer>
#include <QDebug>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothsocket.h>
#include <QBluetoothAddress>
#include <QBluetoothDeviceInfo>
#include <QBluetoothServer>
#include <QBluetoothLocalDevice>

class BluetoothFormatImage : public QObject
{
    Q_OBJECT
private:
    QByteArray dataImage;
public:
    BluetoothFormatImage(const QByteArray &data);
    BluetoothFormatImage(QPixmap &mapImage);
    QByteArray getDataImage(){return dataImage;};
    void saveByteArrayAsImage(QByteArray &byteArray);
};

#endif // BLUETOOTHFORMATIMAGE_H
