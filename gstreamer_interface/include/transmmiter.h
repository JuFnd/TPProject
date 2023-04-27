#ifndef TRANSMITTER_H
#define TRANSMITER_H

#include <QThread>
#include <QString>


extern "C" {
    #include <glib-2.0/glib.h>
    #include <gst/gst.h>
    #include <gst/video/videooverlay.h>
}

class Transmiter : public QThread
{
public:
    // constructor
    // set name using initializer
    explicit Transmiter(QString port_to_transmit);

    // overriding the QThread's run() method
    void run();
    int start_transmit();
    gboolean on_bus_message (GstBus *bus, GstMessage *message, gpointer user_data);

private:
    QString ip;
    QString port;
};


#endif // TRANSMITER_H
