#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <math.h>
#include "app_ds.h"

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
GdkPixbuf *redo;
struct Stack *undo;
cairo_surface_t *surface;
cairo_t *context;
double start_click = 0;

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

gboolean ctrl_z(GtkWidget* widget)
{
/*
	This event handler is for ctrl+w, ctrl+y, ctrl+z operatins on the window.
	It is defined over here as it affects the sheet.
*/
	if(!is_empty_stack(undo))
        {
		redo = gdk_pixbuf_copy(surface_pixbuf);
		surface_pixbuf = gdk_pixbuf_copy(pop_stack(&undo));
                surface = gdk_cairo_surface_create_from_pixbuf (surface_pixbuf, 1, NULL);
                context = cairo_create(surface);
                cairo_set_source_surface(context, surface, 0, 0);
                printf("ctrl z\n");
	}

        return TRUE;	    
}

gboolean ctrl_y(GtkWidget* widget)
{
	undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
        surface_pixbuf = gdk_pixbuf_copy(redo);
        surface = gdk_cairo_surface_create_from_pixbuf (surface_pixbuf, 1, NULL);
        printf("ctrl y\n");
	return TRUE;
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
size_t rectangled = 0;
size_t cheat_bucket = 1;
size_t bucketed = 0;
size_t erased = 0;
void return_draw()
{
  rectangled = 0;
  bucketed = 0;
  erased = 0;
  filename = NULL;
}

void website_button()
{
    printf("Opening website...\n");
    int a = system("xdg-open https://akaagi.github.io/Pinte_Website/accueil.html &");
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
  rectangled = 0;
  bucketed = 0;
  erased = 1;
}

void value_changed(GtkWidget *scale, gpointer user_data) {
    
   size1 = gtk_range_get_value(GTK_RANGE(scale));
   
}

void flood_fill()
{
  erased = 0;
  rectangled = 0;
  bucketed = 1;
}

void get_rect()
{
  erased = 0;
  bucketed = 0;
  rectangled = 1;
}

//rectangle temporaire pour test le bucket
void draw_rectangle()
{
  cairo_t *cr = cairo_create(surface);
  cairo_set_source_rgb(cr, red, green, blue);
  cairo_set_line_width(cr, 5);
  int left_X = 10;
  int right_X = 50;
  int top_Y = 10;
  int bot_Y = 50;
  cairo_move_to(cr, left_X, bot_Y);
  cairo_line_to(cr, right_X, bot_Y);
  cairo_stroke(cr);

  cairo_move_to(cr, left_X, top_Y);
  cairo_line_to(cr, right_X, top_Y);
  cairo_stroke(cr);

  cairo_move_to(cr, left_X, bot_Y+2);
  cairo_line_to(cr, left_X, top_Y-2);
  cairo_stroke(cr);

  cairo_move_to(cr, right_X, bot_Y+2);
  cairo_line_to(cr, right_X, top_Y-2);
  cairo_stroke(cr);
}

gboolean on_click(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
  if (rectangled == 1)
    {
      draw_rectangle();
    }
  else if (bucketed == 1)
    {
      if (cheat_bucket == 1)
	{
	  if(GDK_BUTTON_PRESS)
	    {
	      cairo_t *context = cairo_create(surface);
	      cairo_set_line_width(context, 1);

	      if(acc != 0)
		{
		  previousX = mouseX;
		  previousY = mouseY;
		  surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
		}
	      GdkEventMotion * e = (GdkEventMotion *) event;
	      if (acc == 0)
		{
		  cairo_set_source_rgb(context, red, green, blue);
		  previousX = e->x;
		  previousY = e->y;
		  double tmpX = previousX;
		  double tmpY = previousY;
		  while (previousX < gtk_widget_get_allocated_width(widget))
		    {
		      while (previousY < gtk_widget_get_allocated_height(widget))
			{
			  cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
			  previousY += 1;
			}
		      previousY = tmpY-1;
		      while (previousY > 0)
			{
			  cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
			  previousY -= 1;
			}
		      previousX += 1;
		      previousY = tmpY;
		    }
		  previousX = tmpX;
		  while (previousX > 0)
		    {
		      while (previousY < gtk_widget_get_allocated_height(widget))
			{
			  cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
			  previousY += 1;
			}
		      previousY -= 1;
		      while (previousY > 0)
			{
			  cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
			  previousY -= 1;
			}
		      previousX -= 1;
		      previousY = tmpY;
		    }
		}
	      mouseX= e->x;
	      mouseY = e->y;

	      //cairo_set_source_rgb(context, , 0.5, 0.5);
	      cairo_stroke(context);

	      cairo_destroy(context);

	      gtk_widget_queue_draw_area(widget, 0, 0,
					 gtk_widget_get_allocated_width(widget),
					 gtk_widget_get_allocated_height(widget));
	      cheat_bucket = 0;
	      return TRUE;
	    }
	}
      else if(GDK_BUTTON_PRESS)
	{
	  cairo_t *context = cairo_create(surface);
	  cairo_set_line_width(context, 1);

	  if(acc != 0)
            {
	      previousX = mouseX;
	      previousY = mouseY;
	      surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
            }
	  GdkEventMotion * e = (GdkEventMotion *) event;
	  if (acc == 0)
            {
	      cairo_set_source_rgb(context, red, green, blue);
	      previousX = e->x;
	      previousY = e->y;
	      double tmpX = previousX;
	      double tmpY = previousY;
	      guchar *origine = gdk_pixbuf_get_pixels(surface_pixbuf);
	      guchar *pixel;
	      while (previousX < gtk_widget_get_allocated_width(widget))
		{
		  surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,previousX,previousY,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
		  pixel = gdk_pixbuf_get_pixels(surface_pixbuf);
		  if (pixel[0] == origine[0] && pixel[1] == origine[1] && pixel[2] == origine[2])
		    {
		      cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
		    }
		  else
		    {			cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
		      break;
		    }
		  while (previousY < gtk_widget_get_allocated_height(widget))
		    {
		      surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,previousX,previousY,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
		      pixel = gdk_pixbuf_get_pixels(surface_pixbuf);
		      if (pixel[0] == origine[0] && pixel[1] == origine[1] && pixel[2] == origine[2])
			{
			  cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
			  previousY += 1;
			}
		      else
			{
			  cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
			  break;
			}
		    }
		  previousY = tmpY-1;
		  while (previousY > 0)
		    {
		      surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,previousX,previousY,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
		      pixel = gdk_pixbuf_get_pixels(surface_pixbuf);
		      if (pixel[0] == origine[0] && pixel[1] == origine[1] && pixel[2] == origine[2])
			{
			  cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
			  previousY -= 1;
			}
		      else
			{
			  cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
			  break;
			}
		    }
		  previousX += 1;
		  previousY = tmpY;
		}
	      previousX = tmpX;
	      previousY = tmpY;
	      while (previousX > 0)
		{
		  surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,previousX,previousY,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
		  pixel = gdk_pixbuf_get_pixels(surface_pixbuf);
		  if (pixel[0] == origine[0] && pixel[1] == origine[1] && pixel[2] == origine[2])
		    {
		      cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
		    }
		  else
		    {
		      cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
		      break;
		    }
		  while (previousY < gtk_widget_get_allocated_height(widget))
		    {
		      surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,previousX,previousY,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
		      pixel = gdk_pixbuf_get_pixels(surface_pixbuf);
		      if (pixel[0] == origine[0] && pixel[1] == origine[1] && pixel[2] == origine[2])
			{
			  cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
			  previousY += 1;
			}
		      else
			{
			  cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
			  break;
			}
		    }
		  previousY -= 1;
		  while (previousY > 0)
		    {
		      surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,previousX,previousY,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
		      pixel = gdk_pixbuf_get_pixels(surface_pixbuf);
		      if (pixel[0] == origine[0] && pixel[1] == origine[1] && pixel[2] == origine[2])
			{
			  cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
			  previousY -= 1;
			}
		      else
			{
			  cairo_rectangle(context, previousX, previousY, size1/40, size1/40);
			  break;
			}
		    }
		  previousX -= 1;
		  previousY = tmpY;
		}
            }
	  mouseX= e->x;
	  mouseY = e->y;

	  //cairo_set_source_rgb(context, , 0.5, 0.5);
	  cairo_stroke(context);

	  cairo_destroy(context);

	  gtk_widget_queue_draw_area(widget, 0, 0,
				     gtk_widget_get_allocated_width(widget),
				     gtk_widget_get_allocated_height(widget));
            
	  return TRUE;
	}
    }
  else if (erased == 0) 
    {
        cairo_t *context = cairo_create(surface);

        if(GDK_BUTTON_PRESS)
        {
            if (start_click == 1)
            {
              undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
              start_click = 0;
            }
            //printf("test\n");
            cairo_set_line_width(context, size1);


            if(acc != 0)
            {
                previousX = mouseX;
                previousY = mouseY;
                surface_pixbuf = gdk_pixbuf_get_from_surface(surface,0,0,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
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
             if (start_click == 1)
            {
              undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
              start_click = 0;
            }

            //printf("test\n");
            cairo_t *context = cairo_create(surface);
            cairo_set_line_width(context, 10);


            if(acc != 0)
            {
                previousX = mouseX;
                previousY = mouseY;
                surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
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
    printf("in stack\n");
    acc = 0;
    start_click = 1;
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
    GtkButton *retour;
    GtkButton *annul;
    GtkButton *bucket;
    GtkButton *rect;

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
    retour = GTK_BUTTON(gtk_builder_get_object(builder, "return"));
    CHECK(retour)
    annul = GTK_BUTTON(gtk_builder_get_object(builder, "cancel"));
    CHECK(annul)
    bucket = GTK_BUTTON(gtk_builder_get_object(builder, "bucket"));
    CHECK(bucket)
    rect =  GTK_BUTTON(gtk_builder_get_object(builder, "rectangle"));
    CHECK(rect)
		

    hscale = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, GTK_ADJUSTMENT(adjustement));
    gtk_container_add(GTK_CONTAINER(grid), hscale);

    //g_signal_connect(G_OBJECT(drawarea), "draw",G_CALLBACK(on_draw_event), NULL); 

    gtk_widget_add_events(drawarea, 
            GDK_BUTTON_PRESS_MASK |
            GDK_BUTTON_MOTION_MASK |
            GDK_BUTTON_RELEASE_MASK |
            GDK_KEY_PRESS_MASK);
    g_signal_connect(load, "clicked", G_CALLBACK(openfile), NULL);
    g_signal_connect(pen, "clicked", G_CALLBACK(return_draw), NULL);
    g_signal_connect(erase, "clicked", G_CALLBACK(erase_white), NULL);
    g_signal_connect(bucket, "clicked", G_CALLBACK(flood_fill), NULL);
    g_signal_connect(rect, "clicked", G_CALLBACK(get_rect), NULL);
    g_signal_connect(drawarea, "button-press-event", G_CALLBACK(on_click), NULL); //blc
    g_signal_connect(drawarea, "motion-notify-event", G_CALLBACK(on_click), NULL); //important
    g_signal_connect(drawarea, "button-release-event", G_CALLBACK(on_click_release), NULL);
    g_signal_connect(color1, "color-set", G_CALLBACK(on_color1_color_set), NULL);
    g_signal_connect(G_OBJECT(drawarea), "draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(web, "activate", G_CALLBACK(website_button), NULL);
    g_signal_connect(save, "clicked", G_CALLBACK(on_save),NULL);
    g_signal_connect(hscale, "value-changed", G_CALLBACK(value_changed), NULL);
    g_signal_connect(retour, "clicked", G_CALLBACK(ctrl_z), NULL);
    g_signal_connect(annul, "clicked", G_CALLBACK(ctrl_y), NULL);


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
    surface_pixbuf = gdk_pixbuf_get_from_surface(surface,0,0,gtk_widget_get_allocated_width(drawarea),gtk_widget_get_allocated_height(drawarea));
    undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
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
