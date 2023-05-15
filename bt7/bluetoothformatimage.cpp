#include "bluetoothformatimage.h"

BluetoothFormatImage::BluetoothFormatImage(QPixmap &mapImage){
    QBuffer buffer(&dataImage);
    buffer.open(QIODevice::WriteOnly);
    mapImage.save(&buffer, "PNG");
    qDebug() << dataImage;
}
