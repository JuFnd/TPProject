#include "bluetoothformatimage.h"
#include <QDir>

BluetoothFormatImage::BluetoothFormatImage(const QByteArray &data)
{
    dataImage = data;
}

BluetoothFormatImage::BluetoothFormatImage(QPixmap &mapImage){
    QBuffer buffer(&dataImage);
    buffer.open(QIODevice::WriteOnly);
    mapImage.save(&buffer, "PNG");
}

void BluetoothFormatImage::saveByteArrayAsImage(QByteArray &byteArray)
{
    QString fileName = "screenshot.png";
    QPixmap pixmap;
    pixmap.loadFromData(byteArray);

    QString filePath = QString("%1/%2").arg(QDir::currentPath()).arg(fileName);

    QBuffer buffer(&byteArray);
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    pixmap.save(&file, "PNG");
}
