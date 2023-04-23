#ifndef RECIVER_H
#define RECIVER_H

#include <QThread>
#include <QString>
#include <QDebug>

#include <stdio.h>
//#include <gst/gst.h>

//static gboolean on_bus_message (GstBus *bus, GstMessage *message, gpointer user_data);
//int start_reciver();

class thread
{
public:
    // constructor
    // set name using initializer
    explicit thread(QString s);

    int HW();
private:
    QString name;
};



#endif // RECIVER_H
