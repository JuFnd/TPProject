#include "mainwindow.h"
#include "bluetoothformatimage.h"


void bluetoothLocalList(QApplication &a){
    QBluetoothLocalDevice localDevice;
    QList<QBluetoothAddress> addresses = localDevice.connectedDevices();
    qDebug() << localDevice.address().toString();
    // Iterate over all connected device addresses and get their information
    QList<QBluetoothDeviceInfo> devices;
    for (const auto& address : addresses) {
        QBluetoothDeviceInfo deviceInfo(address,address.toString(),address.toUInt64());
        devices.append(deviceInfo);
        qDebug() << "Connected Device Address:" << deviceInfo.address().toString();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    bluetoothLocalList(a);
    w.setImg();
    //if(w.getImg() != nullptr) sendMessage(w.getImg());
    w.show();
    return a.exec();
}
