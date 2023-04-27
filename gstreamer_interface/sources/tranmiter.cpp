#include "transmmiter.h"
#include <QDebug>

Transmiter::Transmiter(QString port_to_transmit) : port(port_to_transmit)
{
}

void Transmiter::run()
{
    this->start_transmit();
}


//gboolean Transmiter::on_bus_message (GstBus *bus, GstMessage *message, gpointer user_data)
//{
//    GError *error = NULL;
//    gchar *debug_info = NULL;

//    switch (GST_MESSAGE_TYPE (message)) {
//    case GST_MESSAGE_ERROR:
//        gst_message_parse_error (message, &error, &debug_info);
//        g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (message->src), error->message);
//        g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
//        g_clear_error (&error);
//        g_free (debug_info);
//        break;
//    case GST_MESSAGE_WARNING:
//        gst_message_parse_warning (message, &error, &debug_info);
//        g_printerr ("Warning received from element %s: %s\n", GST_OBJECT_NAME (message->src), error->message);
//        g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
//        g_clear_error (&error);
//        g_free (debug_info);
//        break;
//    default:
//        break;
//    }

//    return TRUE;
//}

int Transmiter::start_transmit() {
    GstElement *pipeline = nullptr;
    GstBus *bus = nullptr;
    GstMessage *msg = nullptr;

    gchar *pipeline_str = "ximagesrc ! video/x-raw,framerate=20/1 ! videoscale ! videoconvert ! x264enc tune=zerolatency bitrate=500 speed-preset=superfast ! rtph264pay name=pay0 pt=96 ! udpsink host=127.0.0.1 port=5000 alsasrc  ! audioconvert ! audioresample ! opusenc ! rtpopuspay name=pay1 pt=97 ! udpsink host=127.0.0.1 port=5001";

    // gstreamer initialization
    gst_init(NULL, NULL);

    // building pipeline
    pipeline = gst_parse_launch(
        pipeline_str,
        nullptr);

    // start playing
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    //wait until error or EOS ( End Of Stream )
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                     static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    // free memory
    if (msg != nullptr)
        gst_message_unref(msg);
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}
