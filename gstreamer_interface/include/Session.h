#ifndef SESSION_H
#define SESSION_H

#include <QThread>
#include <QString>
#include <gst/gst.h>



class Session : public QThread
{
public:
    // overriding the QThread's run() method
    void run() = 0;
    void start_sesion();
    void stop_sesion();

protected:
    gboolean bus_callback(GstBus *bus, GstMessage *msg, gpointer data);

    struct CustomData {
        GstElement *pipeline;
        GstBus *bus;
        GstMessage *msg;
        GMainLoop *loop;
    } data;

    QString ip;//fix_it local_ip
    int port_for_audio;
    int port_for_video;

};
#endif // SESSION_H
