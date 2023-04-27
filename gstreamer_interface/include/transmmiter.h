#ifndef TRANSMITTER_H
#define TRANSMITER_H

#include <QThread>
#include <QString>
#include <Session.h>
#include <gst/gst.h>

class Transmiter : public Session
{
public:
    // constructor
    // set name using initializer
    explicit Transmiter(QString port_to_transmit);

    // overriding the QThread's run() method
    void run();
    int start_transmit();

private:
    QString ip;
    QString port;
};


#endif // TRANSMITER_H
