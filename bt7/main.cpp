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


void sendMessage(BluetoothFormatImage *data) {
    // Set up the Bluetooth socket
    QBluetoothSocket socket(QBluetoothServiceInfo::RfcommProtocol);
    QBluetoothAddress address("0A:62:21:20:18:AA");
    quint16 port = 5777; // Default port for RFCOMM protocol

    // Connect to the remote device
    socket.connectToService(address, port);

    // Write the message to the socket
    QByteArray message = data->getDataImage();
    socket.write(message);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    bluetoothLocalList(a);
    w.setImg();
    if(w.getImg() != nullptr) sendMessage(w.getImg());
    w.show();
    return a.exec();
}
