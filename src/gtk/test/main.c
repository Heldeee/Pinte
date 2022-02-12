#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <math.h>

#define CHECK(pointer) \
        if(pointer == NULL) \
            exit(1);


int size1 = 1;
gchar filename1;
const char* filename;

void openfile(GtkButton *button, gpointer user_data)
{
    button = button;
    g_print("Loading...\n");

    GtkWidget* toplevel = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkFileFilter *filter = gtk_file_filter_new ();
    GtkWidget* dialog = gtk_file_chooser_dialog_new(("Open image"),
        GTK_WINDOW(toplevel),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "Open", GTK_RESPONSE_ACCEPT,
        "Cancel", GTK_RESPONSE_CANCEL,
        NULL);

    gtk_file_filter_add_pixbuf_formats (filter);
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),filter);

    switch (gtk_dialog_run(GTK_DIALOG(dialog)))
    {
    case GTK_RESPONSE_ACCEPT:
    {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        break;
    }
    default:
        break;
    }
    gtk_widget_destroy(dialog);
}



cairo_surface_t *surface;
cairo_t *context;

gboolean on_draw(GtkWidget *widget, cairo_t *context, gpointer user_data)
{
    cairo_set_source_rgba(context, 0.5, 0.5, 0.1,1);
    cairo_set_source_surface(context, surface, 0, 0);
    cairo_paint(context);
    return TRUE;

}

void show(GtkWidget *widget, cairo_t *cr){

    GdkPixbuf *pixbuf;

    pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    printf("%s\n", filename);    

    //===Set Source===//
    gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);

    //===Fill===//  
    cairo_fill(cr);

    return;
}


double mouseX;
double mouseY;
double previousX, previousY;
int acc = 0;
double red = 0;
double green = 0;
double blue = 0;
GdkRGBA couleur;


void on_color1_color_set(GtkColorButton *cb)
{
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(cb), &couleur);
    red = couleur.red;
    green = couleur.green;
    blue = couleur.blue;
}

gboolean on_click(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{   
   
    if(GDK_BUTTON_PRESS)
    {
        cairo_t *context = cairo_create(surface);
        cairo_set_line_width(context, size1);

        
        if(acc != 0)
        {
                previousX = mouseX;
                previousY = mouseY;
        }
        GdkEventMotion * e = (GdkEventMotion *) event;
        if (acc == 0)
        {
            previousX = e->x;
            previousY = e->y;
            cairo_rectangle(context, previousX, previousY, size1, size1);
            cairo_fill(context);
        }
        mouseX= e->x;
        mouseY = e->y;
    

        cairo_set_source_rgb(context, red, green, blue);
        cairo_move_to(context, previousX, previousY);
        cairo_line_to(context, mouseX, mouseY);
        cairo_stroke(context);
        acc = 1;


        cairo_destroy(context);

        gtk_widget_queue_draw_area(widget, 0, 0,
                gtk_widget_get_allocated_width(widget),
                gtk_widget_get_allocated_height(widget));
        return TRUE;
    }


    return FALSE;
}

gboolean on_click_release(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    //printf("ZIZOU2\n");
    acc = 0;
    return TRUE;
}



void create_window(GtkApplication *app, gpointer data)
{
    GtkWidget *window;
    GtkWidget *drawarea;
    GtkWidget *color1;
    GtkButton *load;
    GtkButton *show;


    GtkBuilder *builder = gtk_builder_new_from_file("pinte.glade");
    CHECK(builder)
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    CHECK(window)
    drawarea = GTK_WIDGET(gtk_builder_get_object(builder, "drawarea"));
    CHECK(drawarea)
    color1 = GTK_WIDGET(gtk_builder_get_object(builder, "color1"));
    CHECK(color1)
    load = GTK_BUTTON(gtk_builder_get_object(builder, "load"));
    CHECK(load)
    show = GTK_BUTTON(gtk_builder_get_object(builder, "show"));
    CHECK(show)




    gtk_widget_add_events(drawarea, 
            GDK_BUTTON_PRESS_MASK |
            GDK_BUTTON_MOTION_MASK |
            GDK_BUTTON_RELEASE_MASK);
    g_signal_connect(load, "clicked", G_CALLBACK(openfile), NULL);
    g_signal_connect(drawarea, "button-press-event", G_CALLBACK(on_click), NULL); //blc
    g_signal_connect(drawarea, "motion-notify-event", G_CALLBACK(on_click), NULL); //important
    g_signal_connect(drawarea, "button-release-event", G_CALLBACK(on_click_release), NULL);
    g_signal_connect(color1, "color-set", G_CALLBACK(on_color1_color_set), NULL);
    g_signal_connect(show, "clicked", G_CALLBACK(show), NULL);
    g_signal_connect(G_OBJECT(drawarea), "draw", G_CALLBACK(on_draw), NULL);


    //g_signal_connect(drawarea, "draw", G_CALLBACK(on_draw), NULL); useless
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
