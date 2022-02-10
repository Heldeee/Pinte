#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <math.h>

#define CHECK(pointer) \
        if(pointer == NULL) \
            exit(1);
int size1 = 1;

cairo_surface_t *surface;
cairo_t *context;

gboolean on_draw(GtkWidget *widget, cairo_t *context, gpointer user_data)
{
    cairo_set_source_rgba(context, 0.5, 0.5, 0.1,1);
    cairo_set_source_surface(context, surface, 0, 0);
    cairo_paint(context);
    return TRUE;

}

/*on_update_button_clicked(GtkButton *Update_Button)
{
     //2nd argument is serial_data function which contain actual data    
     g_timeout_add(250,serial_data,NULL); 
}*/

double mouseX;
double mouseY;
double previousX, previousY;
int acc = 0;
double red = 0;
double green = 0;
double blue = 0;


gboolean on_click(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{   
   
    //cairo_t *context = cairo_create(surface);
    //cairo_set_line_width(context, size1);
    //cairo_set_source_rgb(context,50/255,50/255,50/255);
    if(GDK_BUTTON_PRESS)
    {
        cairo_t *context = cairo_create(surface);
        cairo_set_source_rgba(context,50/255,50/255,50/255, 1);
        cairo_set_line_width(context, size1);

        printf("debut");
        
        if(acc != 0)
        {
                previousX = mouseX;
                previousY = mouseY;
        }
        GdkEventMotion * e = (GdkEventMotion *) event;
        if (acc == 0)
        {
            cairo_set_source_rgba(context,50/255,50/255,50/255, 1);
            previousX = e->x;
            previousY = e->y;
            cairo_rectangle(context, previousX, previousY, size1, size1);
            cairo_fill(context);
        }
        mouseX= e->x;
        mouseY = e->y;
    
        if (event->button ==1)
        {
            red = 0;
            green = 0;
            blue = 0;
            

            printf("adugasduk\n");
        }
        if (event->button == 3)
        {
            blue = 1;
            red = 1;
            green = 1;
            printf("uiaguiqwdqwiohioq\n");
         

        }
        printf("zizou\n");

        //cairo_set_source_rgb(context, 0, 0, 0);
        //cairo_set_line_width(context, size1);
        //printf("PX: %f | PY %f\n", previousX, previousY);
        //printf("X: %f | Y %f\n", mouseX, mouseY);
        cairo_set_source_rgb(context,red,green,blue);
        cairo_move_to(context, previousX, previousY);
        cairo_line_to(context, mouseX, mouseY);
        cairo_stroke(context);
        acc = 1;
        //printf("%i\n", acc);
        //cairo_rectangle(context, mouseX, mouseY, 3, 3);
        //cairo_fill(context);


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

    GtkBuilder *builder = gtk_builder_new_from_file("pinte.glade");
    CHECK(builder)
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    CHECK(window)
    drawarea = GTK_WIDGET(gtk_builder_get_object(builder, "drawarea"));
    CHECK(drawarea)

    //gtk_widget_add_events(drawarea, GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(drawarea, 
            GDK_BUTTON_PRESS_MASK |
            GDK_BUTTON_MOTION_MASK |
            GDK_BUTTON_RELEASE_MASK);
    g_signal_connect(drawarea, "button-press-event", G_CALLBACK(on_click), NULL); //blc
    g_signal_connect(drawarea, "motion-notify-event", G_CALLBACK(on_click), NULL); //important
    g_signal_connect(drawarea, "button-release-event", G_CALLBACK(on_click_release), NULL);
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
