#include "reciver.h"
#include <QDebug>


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




Reciver::Reciver(QString port_to_reciving):Session()
{
    port = port_to_reciving;
    /* Initialize our data structure */
    memset (&data, 0, sizeof (data));
    qDebug() << "port:" << this->port;
}

void Reciver::run()
{
    this->start_reciver();
}


int Reciver::start_reciver()
{
    GstElement *pipeline, *udpsrc1, *udpsrc2, *depay1, *depay2, *parse1, *parse2, *decode1, *decode2, *convert1, *convert2, *autovideosink1, *autovideosink2, *audioresample, *capsfilter1, *capsfilter2, *queue1, *queue2;
    GstBus *bus;
    GstMessage *msg;
    GstCaps *caps1, *caps2;
    GMainLoop *loop;

    gst_init(NULL, NULL);

    pipeline = gst_pipeline_new("pipeline");
    udpsrc1 = gst_element_factory_make("udpsrc", "udpsrc1");
    queue1 = gst_element_factory_make("queue", "buffer1");
    capsfilter1 = gst_element_factory_make("capsfilter", "capsfilter1");
    depay1 = gst_element_factory_make("rtph264depay", "depay1");
    parse1 = gst_element_factory_make("h264parse", "parse1");
    decode1 = gst_element_factory_make("avdec_h264", "decode1");
    convert1 = gst_element_factory_make("videoconvert", "convert1");
    autovideosink1 = gst_element_factory_make("autovideosink", "autovideosink1");

    udpsrc2 = gst_element_factory_make("udpsrc", "udpsrc2");
    queue2 = gst_element_factory_make("queue", "buffer2");
    capsfilter2 = gst_element_factory_make("capsfilter", "capsfilter2");
    depay2 = gst_element_factory_make("rtpopusdepay", "depay2");
    parse2 = gst_element_factory_make("opusparse", "parse2");
    decode2 = gst_element_factory_make("avdec_opus", "decode2");
    convert2 = gst_element_factory_make("audioconvert", "convert2");
    audioresample = gst_element_factory_make("audioresample", "audioresample");
    autovideosink2 = gst_element_factory_make("autoaudiosink", "autovideosink2");


    if (!pipeline || !udpsrc1 || !udpsrc2 || !depay1 || !depay2 || !parse1 || !parse2 || !decode1 || !decode2 || !convert1 || !convert2 || !autovideosink1 || !autovideosink2 || !audioresample || !capsfilter2 || !capsfilter1 || !queue1 || !queue2) {
        g_printerr("Not all elements could be created. Exiting.\n");
        return -1;
    }


    caps1 = gst_caps_new_simple("application/x-rtp",
                                "media",G_TYPE_STRING ,"video",
                                "clock-rate",G_TYPE_INT ,90000,
                                "encoding-name", G_TYPE_STRING, "H264",
                                "payload",G_TYPE_INT ,96,
                                NULL);

    caps2 = gst_caps_new_simple("application/x-rtp",
                                "media",G_TYPE_STRING ,"audio",
                                "clock-rate",G_TYPE_INT ,48000,
                                "encoding-name", G_TYPE_STRING, "OPUS",
                                "payload",G_TYPE_INT ,96,
                                NULL);

    g_object_set(G_OBJECT(capsfilter1), "caps", caps1, NULL);
    g_object_set(G_OBJECT(capsfilter2), "caps", caps2, NULL);

    gst_caps_unref(caps1);
    gst_caps_unref(caps2);


    GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "pipeline_RECEIVE");
    gst_bin_add_many(GST_BIN(pipeline), udpsrc1,queue1 , capsfilter1, depay1, parse1, decode1, convert1, autovideosink1, udpsrc2, queue2, capsfilter2, depay2, parse2, decode2, convert2, audioresample, autovideosink2, NULL);

    GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "pipeline_RECEIVE");

    if (!gst_element_link_many(udpsrc1, queue1 ,capsfilter1 , depay1, parse1, decode1, convert1, autovideosink1, NULL) ||
        !gst_element_link_many(udpsrc2, queue2, capsfilter2 , depay2, parse2, decode2, convert2, audioresample, autovideosink2, NULL)) {
        g_printerr("Could not link all elements. Exiting.\n");
        return -1;
    }

    g_object_set(udpsrc1, "port", 5001, NULL);
    g_object_set(udpsrc2, "port", 5000, NULL);

    GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "pipeline_RECEIVE");
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    loop = g_main_loop_new(NULL, FALSE);

    bus = gst_element_get_bus(pipeline);

//    gst_bus_add_watch(bus, (GstBusFunc)bus_callback, loop);

    gst_object_unref(bus);

    g_main_loop_run(loop);

    g_main_loop_unref(loop);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;

}
