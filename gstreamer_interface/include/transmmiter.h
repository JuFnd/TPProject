#ifndef TRANSMITTER_H
#define TRANSMITER_H

#include <QThread>
#include <QString>
#include <Session.h>
#include <gst/gst.h>
#include <stdio.h>


class Transmiter : public Session
{
public:
    // constructor
    // set name using initializer
    explicit Transmiter(int port_to_transmit_video, int port_to_transmit_audio);

    // overriding the QThread's run() method
    void run();

//    void startSend();

//    gboolean on_bus_message(GstBus *bus, GstMessage *message, gpointer user_data);

    void addLinkVideo();
    void addLinkAudio();
    void startAudioSession();
    void startVideoSession();
    void startSend();
    void killVideoSession();
    void killAudioSession();
};


#endif // TRANSMITER_H
