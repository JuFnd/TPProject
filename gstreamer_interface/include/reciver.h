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

        void startReceive();

        void addLinkVideo() ;

        void addLinkAudio() ;

        void startVideoSession() ;

        void startAudioSession();

        void killVideoSession();

        void killAudioSession();

        void run();
};


#endif // RECIVER_H
