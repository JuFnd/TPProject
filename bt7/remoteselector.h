#ifndef REMOTESELECTOR_H
#define REMOTESELECTOR_H

#include <QtWidgets/qdialog.h>

#include <QtBluetooth/qbluetoothaddress.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothuuid.h>

QT_FORWARD_DECLARE_CLASS(QBluetoothServiceDiscoveryAgent)
QT_FORWARD_DECLARE_CLASS(QListWidgetItem)

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui {
class RemoteSelector;
}
QT_END_NAMESPACE

class RemoteSelector : public QDialog
{
    Q_OBJECT

public:
    explicit RemoteSelector(const QBluetoothAddress &localAdapter, QWidget *parent = nullptr);
    ~RemoteSelector();

    void startDiscovery(const QBluetoothUuid &uuid);
    void stopDiscovery();
    QBluetoothServiceInfo service() const;

private:
    Ui::RemoteSelector *ui;

    QBluetoothServiceDiscoveryAgent *m_discoveryAgent;
    QBluetoothServiceInfo m_service;
    QMap<QListWidgetItem *, QBluetoothServiceInfo> m_discoveredServices;

private slots:
    void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
    void discoveryFinished();
    void on_remoteDevices_itemActivated(QListWidgetItem *item);
    void on_cancelButton_clicked();
};

#endif // REMOTESELECTOR_H
