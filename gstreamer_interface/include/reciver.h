#ifndef RECIVER_H
#define RECIVER_H

#include <QThread>
#include <Session.h>

#include <QString>
#include <gst/gst.h>
#include <string.h>

#include <example_gstreamer.h>

//extern "C" {
//    #include <glib-2.0/glib.h>
//    #include <gst/gst.h>
//    #include <gst/video/videooverlay.h>
//}

//static void cb_message (GstBus *bus, GstMessage *msg, CustomData *data) {

//    switch (GST_MESSAGE_TYPE (msg)) {
//    case GST_MESSAGE_ERROR: {
//        GError* err;
//        gchar* debug;

//        gst_message_parse_error(msg, &err, &debug);
//        g_print("Error: %s\n", err->message);
//        g_error_free(err);
//        g_free(debug);

//        gst_element_set_state(data->pipeline, GST_STATE_READY);
//        g_main_loop_quit(data->loop);
//        break;
//    }
//    case GST_MESSAGE_EOS:
//        /* end-of-stream */
//        gst_element_set_state (data->pipeline, GST_STATE_READY);
//        g_main_loop_quit (data->loop);
//        break;
//    case GST_MESSAGE_BUFFERING: {
//        gint percent = 0;

//        /* If the stream is live, we do not care about buffering. */
//        if (data->is_live) break;

//        gst_message_parse_buffering (msg, &percent);
//        g_print ("Buffering (%3d%%)\r", percent);
//        /* Wait until buffering is complete before start/resume playing */
//        if (percent < 100)
//            gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
//        else
//            gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
//        break;
//    }
//    case GST_MESSAGE_CLOCK_LOST:
//        /* Get a new clock */
//        gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
//        gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
//        break;
//    default:
//        /* Unhandled message */
//        break;
//    }
//}

class Reciver : public Session
{
    public:
        // constructor
        // set name using initializer
        explicit Reciver(QString port_to_reciving);

        // overriding the QThread's run() method
        void run();
//        void cb_message (GstBus *bus, GstMessage *msg, CustomData *data);

//        gboolean on_bus_message (GstBus *bus, GstMessage *message, gpointer user_data);
        int start_reciver();
};


#endif // RECIVER_H
