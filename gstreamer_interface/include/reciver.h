#ifndef RECIVER_H
#define RECIVER_H

#include <QThread>
#include <Session.h>
#include <QString>
#include <gst/gst.h>
#include <string.h>

class Reciver : public Session
{
    public:
        // constructor
        // set name using initializer
        explicit Reciver(QString port_to_reciving);

        // overriding the QThread's run() method
        void run();
        int start_reciver();
};


#endif // RECIVER_H
