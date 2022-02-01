#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#define CHECK(pointer) \
        if(pointer == NULL) \
            exit(1);

cairo_surface_t *surface;


gboolean on_draw(GtkWidget *widget, cairo_t *context, gpointer user_data)
{
    cairo_set_source_surface(context, surface, 0, 0);
    cairo_paint(context);
    return TRUE;

}

gboolean on_motion(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    GdkEventMotion * e = (GdkEventMotion *) event;
    double x = e->x;
    double y = e->y;

    cairo_t *context = cairo_create(surface);

    cairo_rectangle(context, x, y, 10, 10);
    cairo_stroke(context);
    cairo_destroy(context);

    gtk_widget_queue_draw_area(widget, 0, 0,
            gtk_widget_get_allocated_width(widget),
            gtk_widget_get_allocated_height(widget));
    
    return TRUE;
}

void create_window(GtkApplication *app, gpointer data)
{
    GtkWidget *window;
    GtkWidget *drawarea;

    GtkBuilder *builder = gtk_builder_new_from_file("../../data/pinte.glade");
    CHECK(builder)
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    CHECK(window)
    drawarea = GTK_WIDGET(gtk_builder_get_object(builder, "drawarea"));
    CHECK(drawarea)

    gtk_widget_add_events(drawarea, GDK_POINTER_MOTION_MASK);
    g_signal_connect(drawarea, "motion-notify-event", G_CALLBACK(on_motion), NULL);


    g_signal_connect(drawarea, "draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);

    surface = gdk_window_create_similar_surface(
            gtk_widget_get_parent_window(drawarea),
            CAIRO_CONTENT_COLOR,
            gtk_widget_get_allocated_width(drawarea),
            gtk_widget_get_allocated_height(drawarea));


    cairo_t *context = cairo_create(surface);
    cairo_set_source_rgba(context, 1, 1, 1, 1);
    cairo_paint(context);
    cairo_destroy(context);

    gtk_main();
}


int main(int argc, char *argv[])
{
    GtkApplication *app;
    app = gtk_application_new("test.test", G_APPLICATION_FLAGS_NONE);

    g_signal_connect(app, "activate", G_CALLBACK(create_window), NULL);
    int status= g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
