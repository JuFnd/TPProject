#include "reciver.h"

thread::thread(QString s) : name(s)
{
}

int thread::HW(){
    qDebug() << this->name;
    return 0;
}

//static gboolean on_bus_message(GstBus *bus, GstMessage *message, gpointer user_data)
//{
//    GError *error = NULL;
//    gchar *debug_info = NULL;

//    switch (GST_MESSAGE_TYPE(message))
//    {
//    case GST_MESSAGE_ERROR:
//        gst_message_parse_error(message, &error, &debug_info);
//        g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(message->src), error->message);
//        g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
//        g_clear_error(&error);
//        g_free(debug_info);
//        break;
//    case GST_MESSAGE_WARNING:
//        gst_message_parse_warning(message, &error, &debug_info);
//        g_printerr("Warning received from element %s: %s\n", GST_OBJECT_NAME(message->src), error->message);
//        g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
//        g_clear_error(&error);
//        g_free(debug_info);
//        break;
//    default:
//        break;
//    }

//    return TRUE;
//}

//int start_reciver()
//{
//    GstElement *pipeline;
//    GstBus *bus;
//    // gchar *pipeline_str = "udpsrc port=5000 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! decodebin ! videoconvert ! autovideosink";
//    gchar *pipeline_str = "udpsrc port=5000 ! application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96 ! rtph264depay ! decodebin ! videoconvert ! autovideosink";

//    gst_init(NULL,NULL);

//    pipeline = gst_parse_launch(pipeline_str, NULL);

//    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));

//    gst_bus_add_watch(bus, (GstBusFunc)on_bus_message, NULL);

//    gst_element_set_state(pipeline, GST_STATE_PLAYING);

//    gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

//    GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "pipeline_debug5");
//    gst_element_set_state(pipeline, GST_STATE_PLAYING);

//    // bus = gst_element_get_bus(pipeline);
//    // msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

//    // if (msg != NULL)
//    //   gst_message_unref(msg);
//    gst_object_unref(bus);
//    gst_element_set_state(pipeline, GST_STATE_NULL);
//    gst_object_unref(pipeline);

//    return 0;
//}
