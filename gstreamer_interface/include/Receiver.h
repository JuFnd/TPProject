#pragma once

#include <QString>
#include <gst/gst.h>

#include "Session.h"

class ReciverVideo : public Session
{
private:
    Q_OBJECT

    void addLinkVideo();

    void addLinkAudio();

public:
    explicit ReciverVideo(const qint16 video_port, const qint16 audio_port);
    explicit ReciverVideo(const qint16 audio_port);

    ~ReciverVideo();

public slots:
    void onEnableVideo() override;

    void onEnableAudio() override;

    void onDisableVideo() override;

    void onDisableAudio() override;
};
