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
    GstElement *pipeline = nullptr;
    GstBus *bus = nullptr;
    GstMessage *msg = nullptr;
    GstStateChangeReturn ret;
    GMainLoop* main_loop;

    gchar *pipeline_str = "udpsrc port=5000 ! application/x-rtp,media=(string)video,clock-rate=(int)90000,encoding-name=(string)H264,payload=(int)96 ! rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! autovideosink sync=false udpsrc port=5001 ! application/x-rtp,media=(string)audio,clock-rate=(int)48000,encoding-name=(string)OPUS,payload=(int)96 ! rtpopusdepay ! opusparse ! avdec_opus ! audioconvert ! audioresample ! autoaudiosink";


    // gstreamer initialization
    gst_init(NULL, NULL);

    // building pipeline
    pipeline = gst_parse_launch(
        pipeline_str,
        nullptr);

    // start playing
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
        return -1;
    } else if (ret == GST_STATE_CHANGE_NO_PREROLL) {
        data.is_live = TRUE;
    }

    main_loop = g_main_loop_new (NULL, FALSE);
    data.loop = main_loop;
    data.pipeline = pipeline;

    //wait until error or EOS ( End Of Stream )
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                     static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
//    g_signal_connect (bus, "message", G_CALLBACK (cb_message), &data);


    // free memory
    if (msg != nullptr)
        gst_message_unref(msg);
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}
