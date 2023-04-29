#include "mainwindow.h"
#include <QApplication>
#include <QtBluetooth/QBluetoothAddress>
#include <QString>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothDeviceInfo>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QDebug>
#include <QtBluetooth/QBluetoothSocket>
#include <QFile>
#include <QCoreApplication>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QBuffer>
#include <QCoreApplication>
#include <QAudioFormat>
#include <QAudioInput>
#include <QCoreApplication>
#include <QAudioRecorder>
#include <QUrl>
#include <QIODevice>
#include <QMediaRecorder>
#include <QThread>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QBluetoothSocket>
#include <QDataStream>
#include <QIODevice>


QList<QBluetoothDeviceInfo> bluetoothDeviceList(){
    QList<QBluetoothDeviceInfo> devices;

    QBluetoothDeviceDiscoveryAgent discoveryAgent;

    // Подключаем сигнал, который будет вызван, когда найдено новое устройство
    QObject::connect(&discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
                     [&](const QBluetoothDeviceInfo &device){
                         qDebug() << "Найдено устройство:" << device.name() << "Адрес:" << device.address().toString();
                                                                                                                     devices.append(device);
                     });

    // Начинаем поиск устройств
    discoveryAgent.start();

    // Wait for the search to complete
    while(discoveryAgent.isActive()) {
        QCoreApplication::processEvents();
    }

    return devices;
}

void bluetoothDeviceListShow(){
    QBluetoothDeviceDiscoveryAgent discoveryAgent;

    // Подключаем сигнал, который будет вызван, когда найдено новое устройство
    QObject::connect(&discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
                     [&](const QBluetoothDeviceInfo &device){
                         qDebug() << "Найдено устройство:" << device.name() << "Адрес:" << device.address().toString();
                     });

    // Начинаем поиск устройств
    discoveryAgent.start();
}

void bluetoothPairedDeviceCheck(const QBluetoothAddress &address, QBluetoothSocket &socket){
    QBluetoothLocalDevice bluetooth;
    socket.connectToService((QBluetoothAddress)"BC:7A:BF:8F:94:1D", QBluetoothUuid(QBluetoothUuid::AudioSink));
    // Проверяем, сопряжено ли устройство с указанным адресом
    bool paired = bluetooth.pairingStatus(address) == QBluetoothLocalDevice::Paired;

    if (paired) {
        qDebug() << "Устройство с адресом" << address.toString() << "является сопряженным";
    } else {
        qDebug() << "Устройство с адресом" << address.toString() << "не является сопряженным";
    }
}

void sendAudioOverBluetooth(const QString &deviceAddress)
{

    // Set up the audio format
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    QAudioDeviceInfo inputDevice = QAudioDeviceInfo::defaultInputDevice();
    if (!inputDevice.isFormatSupported(format)) {
        // Handle unsupported formats
    }
    // Open the audio input with the specified format
    QAudioInput *audioInput = new QAudioInput(inputDevice, format);
    QIODevice *audioDevice = audioInput->start();
    if (!audioDevice) {
        // Handle audio input error
    }

    // Set up the Bluetooth socket and connect to the device
    QBluetoothSocket socket(QBluetoothServiceInfo::RfcommProtocol);
    socket.connectToService(QBluetoothAddress(deviceAddress), QBluetoothUuid(deviceAddress.toUInt()));

    // Stream audio data over Bluetooth
    QDataStream outputStream(&socket);
    while (audioDevice->isOpen()) {
        qint64 bytesAvailable = audioDevice->bytesAvailable();
        if (bytesAvailable > 0) {
            QByteArray audioData = audioDevice->read(bytesAvailable);
            outputStream.writeRawData(audioData.constData(), audioData.size());
        }
    }

    audioInput->stop();
    delete audioInput;
    audioDevice->close();
    delete audioDevice;
}


void bluetoothDeviceSendInfo(const QBluetoothAddress &address, QBluetoothSocket &socket, QApplication &a){
    // Open audio input device
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning() << "Default format not supported, trying to use the nearest format.";
        format = info.nearestFormat(format);
    }

    QBuffer audioBuffer;
    audioBuffer.open(QIODevice::ReadWrite);

    QAudioInput audioInput(info, format);
    audioInput.start(&audioBuffer);

    // Wait for audio data to be captured
    //audioInput.waitForReadyRead(3000);          // Wait up to 3 seconds

    // Send audio data over Bluetooth
    socket.connectToService(address, QBluetoothUuid(QBluetoothUuid::AudioSink));
    socket.write(audioBuffer.data());

    // Stop recording audio
    audioInput.stop();
    audioBuffer.close();
}


void bluetoothDeviceConnectionCheck(const QBluetoothAddress &address, QBluetoothSocket &socket){
    QList<QBluetoothDeviceInfo> devices = bluetoothDeviceList();
    foreach (const QBluetoothDeviceInfo &device, devices) {
        if (device.address() == address) {
            qDebug() << "Device found:" << device.name() << " " << device.deviceUuid();
            if (device.isValid() && device.isCached()) {
                qDebug() << "Device is connected.";
            } else {
                qDebug() << "Device is not connected.";
            }
        }
    }
}

void bluetoothLocalList(QApplication &a, QBluetoothSocket &socket){
    QBluetoothLocalDevice localDevice;
    //QBluetoothSocket socket(QBluetoothServiceInfo::RfcommProtocol);
    if (localDevice.isValid()) {
        // Получаем адрес локального устройства Bluetooth
        const QBluetoothAddress localDeviceAddress = localDevice.address();

        // Выводим адрес локального устройства в консоль
        qDebug() << "Local device address:" << localDeviceAddress.toString();

        // Получаем список всех устройств, сопряженных с локальным устройством Bluetooth
        const QList<QBluetoothAddress> remoteDevices = localDevice.connectedDevices();
        QBluetoothDeviceInfo remoteDeviceInfo;
        bluetoothDeviceSendInfo(remoteDeviceInfo.address(),socket,a);
        // Выводим информацию обо всех сопряженных устройствах в консоль
        for (const QBluetoothAddress &remoteDeviceAddress : remoteDevices) {
            // Получаем информацию об устройстве Bluetooth
            QBluetoothDeviceInfo remoteDeviceInfo(remoteDeviceAddress,remoteDeviceAddress.toString(),remoteDeviceAddress.toUInt64());
            socket.connectToService(remoteDeviceAddress, QBluetoothUuid(), QIODevice::ReadOnly);
            //bluetoothDeviceSendInfo(remoteDeviceInfo.address(),socket,a);
            sendAudioOverBluetooth(remoteDeviceAddress.toString());
            // Выводим информацию об устройстве в консоль
            qDebug() << "Remote device found:" << remoteDeviceInfo.name() << remoteDeviceInfo.address().toString();
        }
    }

}

void bluetoothDebugInfo(){
    // Создаем экземпляр класса QBluetoothDeviceDiscoveryAgent
    QBluetoothDeviceDiscoveryAgent agent;

    // Подключаем сигнал, который будет вызываться каждый раз, когда обнаруживается новое устройство
    QObject::connect(&agent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
                     [](const QBluetoothDeviceInfo &info){
                         // Выводим информацию об устройстве в консоль
                         qDebug() << "Device found:" << info.name() << info.address().toString();
                     });

    // Запускам обнаружение устройств
    agent.start();

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    bluetoothDeviceListShow();
    QBluetoothSocket socket(QBluetoothServiceInfo::RfcommProtocol);

    // Устанавливаем адрес Bluetooth-устройства, на которое хотим передать данные
    QBluetoothAddress address("B8:F6:53:17:B2:36");
    //bluetoothPairedDeviceCheck(address,socket);
    //bluetoothDeviceSendInfo(address,socket);
    //bluetoothDeviceConnectionCheck(address,socket);
    //bluetoothDebugInfo();
    bluetoothLocalList(a,socket);

    /*QBluetoothAddress senderAddress("BC:7A:BF:8F:94:1D");
    // connect to the sender
    socket.connectToService(senderAddress, QBluetoothUuid(QBluetoothUuid::Headset));
    if (socket.state() != QBluetoothSocket::ConnectedState) {
        // handle error
        return -1;
    }

    // transmit audio to the receiver
    bluetoothDeviceSendInfo(senderAddress, socket, a);

    // disconnect from the sender
    socket.disconnectFromService();*/
    w.show();
    return a.exec();
}
//B8:F6:53:17:B2:36
