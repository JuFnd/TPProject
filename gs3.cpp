#include <gst/gst.h>
#include <gst/video/video.h>

int main (int argc, char *argv[]) {
    gst_init(&argc, &argv);

    /* Создание элементов GStreamer */
    GstElement *screen_source = gst_element_factory_make("ximagesrc", "screen-sour  ce");
    GstElement *video_convert = gst_element_factory_make("videoconvert", "video-convert");
    GstElement *video_sink = gst_element_factory_make("ximagesink", "video-sink");

    /* Создание пайплайна GStreamer */
    GstElement *pipeline1 = gst_pipeline_new("screen-capture-pipeline");
    gst_bin_add_many(GST_BIN(pipeline1), screen_source, video_convert, video_sink, NULL);
    gst_element_link_many(screen_source, video_convert, video_sink, NULL);

    /* Установка параметров захвата экрана */
    g_object_set(G_OBJECT(screen_source), "use-damage", 0, "show-pointer", 1, "screen-num", 0, NULL);

    /* Запуск пайплайна */
    gst_element_set_state(pipeline1, GST_STATE_PLAYING);

    /* Ожидание событий GStreamer */
    GstBus *bus = gst_element_get_bus(pipeline1);
    GstMessage *msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
    if (msg != NULL) {
        gst_message_unref(msg);
    }

    /* Остановка пайплайна */
    gst_element_set_state(pipeline1, GST_STATE_NULL);

    /* Очистка данных */
    gst_object_unref(GST_OBJECT(pipeline1));
    gst_object_unref(GST_OBJECT(bus));
    
    return 0;
}