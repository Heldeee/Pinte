#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <math.h>

#define CHECK(pointer) \
        if(pointer == NULL) \
            exit(1);

struct {
  cairo_surface_t *image;  
} glob;


gchar filename1;
const char* filename;
GtkRange *range;
gdouble size1 =1;   
GdkPixbuf *surface_pixbuf;

void savefile(GtkButton *button, gpointer user_data)
{   
    gdk_pixbuf_save (surface_pixbuf, "snapshot.png", "png", NULL, NULL);
                
}

void on_save(GtkButton *button, gpointer user_data)
{
    button=button;
    GtkWidget* dialog;
    GtkWidget* toplevel = gtk_widget_get_toplevel(GTK_WIDGET(button));
    dialog = gtk_file_chooser_dialog_new("Save Text ",
        GTK_WINDOW(toplevel),
        GTK_FILE_CHOOSER_ACTION_SAVE,
        "Cancel", GTK_RESPONSE_CANCEL,
        "Save", GTK_RESPONSE_ACCEPT,
        NULL);
    switch (gtk_dialog_run(GTK_DIALOG(dialog)))
    {
    case GTK_RESPONSE_ACCEPT:
    {
        gchar* filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        
        gdk_pixbuf_save (surface_pixbuf, filename, "png", NULL, NULL);
        break;
    }
    default:
        break;
    }
    gtk_widget_destroy(dialog);
    
}

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
        glob.image = cairo_image_surface_create_from_png(filename);
        break;
    }
    default:
        break;
    }
    gtk_widget_destroy(dialog);
}

double red = 0;
double green = 0;
double blue = 0;

cairo_surface_t *surface;
cairo_t *context;

//static void do_drawing(cairo_t *cr);

gboolean on_draw(GtkWidget *widget, cairo_t* context ,gpointer user_data)
{   
    //surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
    
    if (filename == NULL)
    {
      
      cairo_set_source_rgba(context, 0.5, 0.5, 0.1,1);
      cairo_set_source_surface(context, surface, 0, 0);
      //do_drawing(context);
      cairo_paint(context);
      surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));

      return TRUE;
    }
    else
    {
    cairo_set_source_surface(context, glob.image, 0, 0);
    cairo_paint(context);
    surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
    return TRUE;
    }
    
}
size_t erased = 0;
void return_draw()
{
    erased = 0;
    filename = NULL;
}

void website_button()
{
    printf("Opening website...\n");
    int a = system("xdg-open https://akaagi.github.io/Pinte_Website/accueil.html");
    if (a)
    {}
}

/*static gboolean on_draw(GtkWidget *da, GdkEvent *event, cairo_t* cr, gpointer data)
{
    (void)event; (void)data;
    GdkPixbuf *pix;
    GError *err = NULL;
    pix = gdk_pixbuf_new_from_file("esfse", &err);
    if(err)
    {
        printf("Error : %s\n", err->message);
        g_error_free(err);
        return FALSE;
    }
    //    cr = gdk_cairo_create (da->window);
    gdk_cairo_set_source_pixbuf(cr, pix, 0, 0);
    cairo_paint(cr);
    //    cairo_fill (cr);
    cairo_destroy (cr);
    return FALSE;
}*/

/*static void do_drawing(cairo_t *cr)
{
  cairo_set_source_surface(cr, glob.image, 10, 10);
  cairo_paint(cr);    
}*/


double mouseX;
double mouseY;
double previousX, previousY;
int acc = 0;
GdkRGBA couleur;

void on_color1_color_set(GtkColorButton *cb)
{
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(cb), &couleur);
    red = couleur.red;
    green = couleur.green;
    blue = couleur.blue;
}

void erase_white()
{
    erased = 1;
}

void value_changed(GtkWidget *scale, gpointer user_data) {
    
   size1 = gtk_range_get_value(GTK_RANGE(scale));
   
}

gboolean on_click(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{   
    
    if (erased == 0) 
    {
        if(GDK_BUTTON_PRESS)
        {
            //printf("test\n");
            
            cairo_t *context = cairo_create(surface);
            cairo_set_line_width(context, size1);


            if(acc != 0)
            {
                previousX = mouseX;
                previousY = mouseY;
                surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
                //gdk_pixbuf_save (surface_pixbuf, "snapshot.png", "png", NULL, NULL);
                
            }
            GdkEventMotion * e = (GdkEventMotion *) event;
            if (acc == 0)
            {
                cairo_set_source_rgb(context, red, green, blue);
                previousX = e->x;
                previousY = e->y;
                //cairo_translate(context, size1/2, size1/2);
                //cairo_arc(context, previousX, previousY, size1, 0, 2*M_PI);
                cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
                //cairo_fill_preserve(context);
                
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
    }
    else
    {
        if(GDK_BUTTON_PRESS)
        {
            //printf("test\n");
            cairo_t *context = cairo_create(surface);
            cairo_set_line_width(context, 10);


            if(acc != 0)
            {
                previousX = mouseX;
                previousY = mouseY;
            }
            GdkEventMotion * e = (GdkEventMotion *) event;
            if (acc == 0)
            {
                cairo_set_source_rgb(context, 1, 1, 1);
                previousX = e->x;
                previousY = e->y;
                //cairo_translate(context, size1/2, size1/2);
                //cairo_arc(context, previousX, previousY, size1, 0, 2*M_PI);
                cairo_rectangle(context, previousX, previousY, 10, 10);
                //cairo_fill_preserve(context);

            }
            mouseX= e->x;
            mouseY = e->y;


            cairo_set_source_rgb(context, 1, 1, 1);
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
    GtkButton *pen;
    GtkButton *erase;
    GtkWidget *web;
    GtkButton *save;
    GtkWidget *hscale;
    GtkPaned *grid;

    GtkAdjustment* adjustement = gtk_adjustment_new(1.0,0.0,10.0,1.0,1.0, 0.0);

    GtkBuilder *builder = gtk_builder_new_from_file("pinte.glade");
    CHECK(builder)
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    CHECK(window)
    drawarea = GTK_WIDGET(gtk_builder_get_object(builder, "drawarea"));
    CHECK(drawarea)
    color1 = GTK_WIDGET(gtk_builder_get_object(builder, "color1"));
    CHECK(color1)
    pen = GTK_BUTTON(gtk_builder_get_object(builder, "pen"));
    CHECK(pen)
    load = GTK_BUTTON(gtk_builder_get_object(builder, "load"));
    CHECK(load)
    erase = GTK_BUTTON(gtk_builder_get_object(builder, "erase"));
    CHECK(erase)
    web = GTK_WIDGET(gtk_builder_get_object(builder, "web"));
    CHECK(web)
    grid = GTK_PANED(gtk_builder_get_object(builder, "grid"));
    //hscale = GTK_WIDGET(gtk_builder_get_object(builder, "erase"));
    //CHECK(hscale)
    save = GTK_BUTTON(gtk_builder_get_object(builder, "save"));
    CHECK(save)
    hscale = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, GTK_ADJUSTMENT(adjustement));
    gtk_container_add(GTK_CONTAINER(grid), hscale);

    //g_signal_connect(G_OBJECT(drawarea), "draw",G_CALLBACK(on_draw_event), NULL); 

    gtk_widget_add_events(drawarea, 
            GDK_BUTTON_PRESS_MASK |
            GDK_BUTTON_MOTION_MASK |
            GDK_BUTTON_RELEASE_MASK);
    g_signal_connect(load, "clicked", G_CALLBACK(openfile), NULL);
    g_signal_connect(pen, "clicked", G_CALLBACK(return_draw), NULL);
    g_signal_connect(erase, "clicked", G_CALLBACK(erase_white), NULL);
    g_signal_connect(drawarea, "button-press-event", G_CALLBACK(on_click), NULL); //blc
    g_signal_connect(drawarea, "motion-notify-event", G_CALLBACK(on_click), NULL); //important
    g_signal_connect(drawarea, "button-release-event", G_CALLBACK(on_click_release), NULL);
    g_signal_connect(color1, "color-set", G_CALLBACK(on_color1_color_set), NULL);
    g_signal_connect(G_OBJECT(drawarea), "draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(web, "activate", G_CALLBACK(website_button), NULL);
    g_signal_connect(save, "clicked", G_CALLBACK(on_save),NULL);
    g_signal_connect(hscale, "value-changed", G_CALLBACK(value_changed), NULL); 


    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);

    surface = gdk_window_create_similar_surface(
            gtk_widget_get_parent_window(drawarea),
            CAIRO_CONTENT_COLOR,
            gtk_widget_get_allocated_width(drawarea),
            gtk_widget_get_allocated_height(drawarea));

    //gtk_widget_set_app_paintable(drawarea, TRUE);

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
