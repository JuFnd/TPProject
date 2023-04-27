#ifndef SESSION_H
#define SESSION_H

extern "C" {
#include <glib-2.0/glib.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
}

typedef struct _CustomData {
    gboolean is_live;
    GstElement* pipeline;
    GMainLoop* loop;
} CustomData;

class Session : public QThread
{
public:
    // overriding the QThread's run() method
    void run() = 0;

protected:
    QString ip;//fix_it local_ip
    QString port;
    CustomData data;

};
#endif // SESSION_H
