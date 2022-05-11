#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <math.h>
#include "app_ds.h"
#include "savepng.h"
#include "../sdl/filter/filtre.h"

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
gdouble size2 = 1;
GdkPixbuf *surface_pixbuf;
GdkPixbuf *redo;
struct Stack *undo;
cairo_surface_t *surface;
cairo_t *context;
double start_click = 0;
GtkWidget* window;
GtkWidget* drawarea;



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
    }

    return TRUE;	    
}

gboolean ctrl_y(GtkWidget* widget)
{
    undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
    surface_pixbuf = gdk_pixbuf_copy(redo);
    surface = gdk_cairo_surface_create_from_pixbuf (surface_pixbuf, 1, NULL);
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
                while(!is_empty_stack(undo))
                {
                    pop_stack(&undo);
                }

                undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
                glob.image = cairo_image_surface_create_from_png(filename);
                surface = glob.image;

                surface_pixbuf = gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));

                //gtk_window_set_default_size(GTK_WINDOW(window), cairo_image_surface_get_width (surface), cairo_image_surface_get_height (surface));

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






static struct Color pixel;
static guchar *current;
struct Queue *front;
int drawareaX;
int drawareaY;
struct Color foreground;
static gint rowstride;
static gint n_channels;
static guchar *pixels;

void floodFill(gint x, gint y, struct Color *color, gint p_size);

void draw_pixel(gint x, gint y, struct Color *color, gint p_size, cairo_t *context){
  /*
    Changes a Pixels color data int the Pixel buffer.
  */
  if(x >= 0 && x < drawareaX && y >= 0 && y < drawareaY){
    if(p_size == 0){
      cairo_rectangle(context, x, y, 1.5, 1.5);
      cairo_fill(context);
      
      current = pixels + y * rowstride + x * n_channels;
      current[0] = color->red;
      current[1] = color->green;
      current[2] = color->blue;
      
    }
    else{
      floodFill(x, y, color, p_size);
    }
  }
}

struct Color *getPixel(gint x, gint y){
  /*
    Returns a Pixel's color data from the Pixel buffer.
  */
  current = pixels + y * rowstride + x * n_channels;
  pixel.red = current[0];
  pixel.green = current[1];
  pixel.blue = current[2];
  return &pixel;
}

gboolean check_pixel(gint x, gint y, struct Color *fgcolor, struct Color *bgcolor, gint p_size, gint ogx, gint ogy){
  if(x >= 0 && x < drawareaX && y >= 0 && y < drawareaY)
    {
      current = pixels + y * rowstride + x * n_channels;
      if(fgcolor->red == current[0] && fgcolor->green == current[1] && fgcolor->blue == current[2])
	{
	  return FALSE;
	}
      if(p_size == 0)
	{
	  if(bgcolor->red == current[0] && bgcolor->green == current[1] && bgcolor->blue == current[2])
	    {
	      return TRUE;
	    }
	}
      else
	{
	  if(abs(x - ogx) <= p_size && abs(y-ogy) <= p_size)
	    return TRUE;
	  return FALSE;
	}
    }
  return FALSE;
}

void floodFill(gint x, gint y, struct Color *color, gint p_size){
  struct Point *current_point;
  gint west, east, valid_north, valid_south;
  struct Color *base_color = getPixel(x,y);
  if(!check_pixel(x, y ,color, base_color, p_size, x, y)){
    if(p_size > 0){
      return;
    }
  }
  front = push_queue(x,y, NULL);

  cairo_t *context = cairo_create(surface);
  cairo_set_source_rgb(context, red, green, blue);
      
  while(!is_queue_empty())
    {
      valid_north = 0;
      valid_south = 0;
      current_point = pop_queue();
      for(west = current_point->x; check_pixel(west, current_point->y, color, base_color, p_size, x, y); west--);
      west--;
      for(east = current_point->x; check_pixel(east, current_point->y, color, base_color, p_size, x, y); east++);
      east++;
      
      for(west = west + 1 ; west < east ; west++)
	{
	  draw_pixel(west, current_point->y, color, 0, context);
	  if(check_pixel(west, current_point->y - 1, color, base_color, p_size, x, y))
	    {
	      if(valid_north == 0)
		{
		  front = push_queue(west, current_point->y - 1, front);
		  valid_north = 1;
		}
	    }
	  else
	    {
	      valid_north = 0;
	    }
	  if(check_pixel(west, current_point->y + 1, color, base_color, p_size, x, y))
	    {
	      if(valid_south == 0)
		{
		  front = push_queue(west, current_point->y + 1, front);
		  valid_south = 1;
		}
	    }
	  else
	    valid_south = 0;
	}
    }

  cairo_destroy(context);
  free(current_point);
  free(front);
}





//static void do_drawing(cairo_t *cr);

/*gboolean on_draw(GtkWidget *widget, cairo_t* context ,gpointer user_data)
{   
  //surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));

  if (filename == NULL)
    {
      cairo_set_source_rgba(context, 0.5, 0.5, 0.1,1);
      cairo_set_source_surface(context, surface, 0, 0);
      //do_drawing(context);
      cairo_paint(context);
     
      return TRUE;
    }
  else
    {
      cairo_set_source_surface(context, glob.image, 0, 0);
      cairo_paint(context);
      printf("%i\n",cairo_image_surface_get_width (glob.image));
      printf("%i\n",cairo_image_surface_get_height(glob.image));
      if (cairo_image_surface_get_width (glob.image)!= 0 && cairo_image_surface_get_height(glob.image)!=0)
	surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (glob.image),cairo_image_surface_get_height(glob.image));
      else
	surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
      return TRUE;
    }
    }*/

gboolean on_draw(GtkWidget *widget, cairo_t* context ,gpointer user_data)
{   
    //surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));

    if (filename == NULL)
    {
        cairo_set_source_rgba(context, 0.5, 0.5, 0.1,1);
        cairo_set_source_surface(context, surface, 0, 0);
        //do_drawing(context);
        cairo_paint(context);
        if (cairo_image_surface_get_width (surface)!= 0 && cairo_image_surface_get_height(surface)!=0)
            surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));
        else
            surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
        return TRUE;
    }
    else
    {
        cairo_set_source_surface(context, glob.image, 0, 0);
        cairo_paint(context);
        if (cairo_image_surface_get_width (surface)!= 0 && cairo_image_surface_get_height(surface)!=0)
            surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));
        else
            surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,gtk_widget_get_allocated_width(widget),gtk_widget_get_allocated_height(widget));
        return TRUE;
    }

}

size_t rectangled = 0;
size_t triangled = 0;
size_t losanged = 0;
size_t circled = 0;
size_t cheat_bucket = 1;
size_t bucketed = 0;
size_t erased = 0;
void return_draw()
{
    rectangled = 0;
    triangled = 0;
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
    triangled = 0;
    losanged = 0;
    circled = 0;
}

void value_changed(GtkWidget *scale, gpointer user_data) {

    size1 = gtk_range_get_value(GTK_RANGE(scale));

}

void value_changed2(GtkWidget *scale, gpointer user_data)
{
    size2 = gtk_range_get_value(GTK_RANGE(scale));
}

void flood_fill()
{
    erased = 0;
    rectangled = 0;
    bucketed = 1;
    triangled = 0;
    losanged = 0;
    circled = 0;
}

void get_rect()
{
    erased = 0;
    bucketed = 0;
    rectangled = 1;
    triangled = 0;
    losanged = 0;
    circled = 0;
}

void get_triangle()
{
    erased = 0;
    bucketed = 0;
    triangled = 1;
    rectangled = 0;
    losanged = 0;
    circled = 0;
}

void get_losange()
{
    erased = 0;
    bucketed = 0;
    triangled = 0;
    rectangled = 0;
    losanged = 1;
    circled = 0;
}

void get_circle()
{   erased = 0;
    bucketed = 0;
    triangled = 0;
    rectangled = 0;
    losanged = 0;
    circled = 1;
}

//rectangle temporaire pour test le bucket
void draw_triangle(int size, GdkEventButton *event)
{
    GdkEventMotion * e = (GdkEventMotion *) event;
    int mousex= e->x;
    int mousey = e->y;
    cairo_t *cr = cairo_create(surface);
    cairo_set_source_rgb(cr, red, green, blue);
    cairo_set_line_width(cr, 5);
    int a_x = mousex;
    int a_y = mousey;
    int b_x = mousex + size;
    int b_y = mousey;
    int c_x = mousex + size/2;
    int c_y = mousey - sqrt((size * size) - (size*size)/4);


    cairo_move_to(cr, a_x, a_y);
    cairo_line_to(cr, b_x, b_y);
    cairo_stroke(cr);

    cairo_move_to(cr, a_x, a_y);
    cairo_line_to(cr, c_x, c_y);
    cairo_stroke(cr);

    cairo_move_to(cr, b_x, b_y);
    cairo_line_to(cr, c_x, c_y);
    cairo_stroke(cr);
}

void draw_losange(int size, GdkEventButton *event)
{
    GdkEventMotion * e = (GdkEventMotion *) event;
    int mousex= e->x;
    int mousey = e->y;
    cairo_t *cr = cairo_create(surface);
    cairo_set_source_rgb(cr, red, green, blue);
    cairo_set_line_width(cr, 5);
    int a_x = mousex;
    int a_y = mousey;
    int b_x = mousex + size;
    int b_y = mousey;
    int c_x = mousex + size/2;
    int c_y = mousey - size/4;
    int d_x = mousex + size/2;
    int d_y = mousey + size/4;


    cairo_move_to(cr, a_x, a_y);
    cairo_line_to(cr, c_x, c_y);
    cairo_stroke(cr);

    cairo_move_to(cr, a_x, a_y);
    cairo_line_to(cr, d_x, d_y);
    cairo_stroke(cr);

    cairo_move_to(cr, c_x, c_y);
    cairo_line_to(cr, b_x, b_y);
    cairo_stroke(cr);

    cairo_move_to(cr, d_x, d_y);
    cairo_line_to(cr, b_x, b_y);
    cairo_stroke(cr);
}


void draw_rectangle(int size, GdkEventButton *event)
{
    GdkEventMotion * e = (GdkEventMotion *) event;
    int mousex= e->x;
    int mousey = e->y;
    cairo_t *cr = cairo_create(surface);
    cairo_set_source_rgb(cr, red, green, blue);
    cairo_set_line_width(cr, 5);
    int left_x = mousex;
    int right_x = mousex + size;
    int top_y = mousey;
    int bot_y = mousey + size;
    cairo_move_to(cr, left_x, bot_y);
    cairo_line_to(cr, right_x, bot_y);
    cairo_stroke(cr);

    cairo_move_to(cr, left_x, top_y);
    cairo_line_to(cr, right_x, top_y);
    cairo_stroke(cr);

    cairo_move_to(cr, left_x, bot_y+2);
    cairo_line_to(cr, left_x, top_y-2);
    cairo_stroke(cr);

    cairo_move_to(cr, right_x, bot_y+2);
    cairo_line_to(cr, right_x, top_y-2);
    cairo_stroke(cr);
}

void draw_circle(int size, GdkEventButton *event)
{
    GdkEventMotion * e = (GdkEventMotion *) event;
    cairo_t *cr = cairo_create(surface);
    cairo_set_source_rgb(cr, red, green, blue);
    cairo_set_line_width(cr, size1);  
    cairo_translate(cr, size1/2, size1/2);
    cairo_arc(cr, e->x, e->y, size2*40, 0, 2 * M_PI);
    cairo_stroke_preserve(cr);
}

gboolean on_click(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{


    if (rectangled == 1)
    {
        undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
        draw_rectangle(100 * size2, event);
    }
    else if (triangled == 1)
    {
        undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
        draw_triangle(100 * size2, event);
    }
    else if (losanged == 1)
    {
        undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
        draw_losange(100 * size2, event);
    }
    else if (circled == 1)
    {
        undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
        draw_circle(100 * size2, event);
    }
    else if (bucketed == 1)
    {
      GdkEventMotion * e = (GdkEventMotion *) event;
      mouseX= e->x;
      mouseY = e->y;

      rowstride = gdk_pixbuf_get_rowstride(surface_pixbuf);
      n_channels = gdk_pixbuf_get_n_channels(surface_pixbuf);
      pixels = gdk_pixbuf_get_pixels(surface_pixbuf);

      drawareaX = gtk_widget_get_allocated_width(drawarea);
      drawareaY = gtk_widget_get_allocated_height(drawarea);
      foreground.red = red*255;
      foreground.green = green*255;
      foreground.blue = blue*255;
      floodFill(mouseX, mouseY, &foreground, 0);
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
                if (cairo_image_surface_get_width (surface)!= 0 && cairo_image_surface_get_height(surface)!=0)
                    surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));
                //surface_pixbuf = gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));
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
                if (cairo_image_surface_get_width (surface)!= 0 && cairo_image_surface_get_height(surface)!=0)
                    surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));
                //surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));
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
    acc = 0;
    start_click = 1;
    return TRUE;
}



gboolean refresh(GtkWidget *widget, GdkEventExpose*event, gpointer user_data)
{
    gtk_widget_queue_draw_area(widget, 0, 0,
            gtk_widget_get_allocated_width(widget),
            gtk_widget_get_allocated_height(widget));
    return TRUE;
}


gboolean loadblank(GtkWidget* widget)
{
    while(!is_empty_stack(undo))
    {
        pop_stack(&undo);
    }

    undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
    glob.image = cairo_image_surface_create_from_png("blank");
    surface = glob.image;
    if (cairo_image_surface_get_width (surface)!= 0 && cairo_image_surface_get_height(surface)!=0)
        surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));


    return TRUE;
}

gboolean grey(GtkWidget *widget)
{
    char* filename = ".temp_filter";
    gdk_pixbuf_save (surface_pixbuf, filename, "png", NULL, NULL);
    init_sdl();
    SDL_Surface *image = load_image(filename);

    __grayscale(image);

    SDL_Surface *bmp = image;
    SDL_SavePNG(bmp, filename);
    SDL_FreeSurface(image);

    undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
    glob.image = cairo_image_surface_create_from_png(filename);
    surface = glob.image;
    if (cairo_image_surface_get_width (surface)!= 0 && cairo_image_surface_get_height(surface)!=0)
        surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));

    return TRUE;
}

gboolean invert(GtkWidget *widget)
{
    char* filename = ".temp_filter";
    gdk_pixbuf_save (surface_pixbuf, filename, "png", NULL, NULL);
    init_sdl();
    SDL_Surface *image = load_image(filename);

    __negative(image);

    SDL_Surface *bmp = image;
    SDL_SavePNG(bmp, filename);
    SDL_FreeSurface(image);

    undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
    glob.image = cairo_image_surface_create_from_png(filename);
    surface = glob.image;
    if (cairo_image_surface_get_width (surface)!= 0 && cairo_image_surface_get_height(surface)!=0)
        surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));

    return TRUE;
}

gboolean cyann(GtkWidget *widget)
{
    char* filename = ".temp_filter";
    gdk_pixbuf_save (surface_pixbuf, filename, "png", NULL, NULL);
    init_sdl();
    SDL_Surface *image = load_image(filename);

    __cyan(image);

    SDL_Surface *bmp = image;
    SDL_SavePNG(bmp, filename);
    SDL_FreeSurface(image);

    undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
    glob.image = cairo_image_surface_create_from_png(filename);
    surface = glob.image;
    if (cairo_image_surface_get_width (surface)!= 0 && cairo_image_surface_get_height(surface)!=0)
        surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));

    return TRUE;
}

gboolean rosee(GtkWidget *widget)
{
    char* filename = ".temp_filter";
    gdk_pixbuf_save (surface_pixbuf, filename, "png", NULL, NULL);
    init_sdl();
    SDL_Surface *image = load_image(filename);

    __rose(image);

    SDL_Surface *bmp = image;
    SDL_SavePNG(bmp, filename);
    SDL_FreeSurface(image);

    undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
    glob.image = cairo_image_surface_create_from_png(filename);
    surface = glob.image;
    if (cairo_image_surface_get_width (surface)!= 0 && cairo_image_surface_get_height(surface)!=0)
        surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));

    return TRUE;
}
gboolean yelloww(GtkWidget *widget)
{
    char* filename = ".temp_filter";
    gdk_pixbuf_save (surface_pixbuf, filename, "png", NULL, NULL);
    init_sdl();
    SDL_Surface *image = load_image(filename);

    __yellow(image);

    SDL_Surface *bmp = image;
    SDL_SavePNG(bmp, filename);
    SDL_FreeSurface(image);

    undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
    glob.image = cairo_image_surface_create_from_png(filename);
    surface = glob.image;
    if (cairo_image_surface_get_width (surface)!= 0 && cairo_image_surface_get_height(surface)!=0)
        surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));

    return TRUE;
}

gboolean BAW(GtkWidget *widget)
{
    char* filename = ".temp_filter";
    gdk_pixbuf_save (surface_pixbuf, filename, "png", NULL, NULL);
    init_sdl();
    SDL_Surface *image = load_image(filename);

    __blackandwhite(image);

    SDL_Surface *bmp = image;
    SDL_SavePNG(bmp, filename);
    SDL_FreeSurface(image);

    undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
    glob.image = cairo_image_surface_create_from_png(filename);
    surface = glob.image;
    if (cairo_image_surface_get_width (surface)!= 0 && cairo_image_surface_get_height(surface)!=0)
        surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));

    return TRUE;
}

gboolean mirrorr(GtkWidget *widget)
{
    char* filename = ".temp_filter";
    gdk_pixbuf_save (surface_pixbuf, filename, "png", NULL, NULL);
    init_sdl();
    SDL_Surface *image = load_image(filename);

    __mirror(image);

    SDL_Surface *bmp = image;
    SDL_SavePNG(bmp, filename);
    SDL_FreeSurface(image);

    undo = push_stack(gdk_pixbuf_copy(surface_pixbuf), undo);
    glob.image = cairo_image_surface_create_from_png(filename);
    surface = glob.image;
    if (cairo_image_surface_get_width (surface)!= 0 && cairo_image_surface_get_height(surface)!=0)
        surface_pixbuf =  gdk_pixbuf_get_from_surface(surface,0,0,cairo_image_surface_get_width (surface),cairo_image_surface_get_height(surface));

    return TRUE;
}



void create_window(GtkApplication *app, gpointer data)
{

    GtkWidget *window;
    //GtkWidget *drawarea;
    GtkWidget *color1;
    GtkWidget *load;
    GtkButton *pen;
    GtkButton *erase;
    GtkWidget *web;
    GtkWidget *save;
    GtkWidget *hscale;
    GtkWidget *hscale2;
    GtkPaned *grid;
    GtkPaned *grid2;
    GtkButton *retour;
    GtkButton *annul;
    GtkButton *bucket;
    GtkButton *rect;
    GtkButton *triangle;
    GtkButton *losange;
    GtkButton *circle;
    GtkWidget *new;
    GtkWidget *menu;
    //FILTERS
    GtkWidget *filter1;
    GtkWidget *filter2;
    GtkWidget *filter3;
    GtkWidget *filter4;
    GtkWidget *filter5;
    GtkWidget *filter6;
    GtkWidget *filter7;

    GtkAdjustment* adjustement = gtk_adjustment_new(1.0,1.0,10.0,1.0,1.0, 0.0);
    GtkAdjustment* adjustement2 = gtk_adjustment_new(1.0,1.0,10.0,1.0,1.0, 0.0);

    GtkBuilder *builder = gtk_builder_new_from_file("pinte.glade");
    CHECK(builder)
        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    CHECK(window)
    triangle = GTK_BUTTON(gtk_builder_get_object(builder, "triangle"));
    CHECK(triangle)
        drawarea = GTK_WIDGET(gtk_builder_get_object(builder, "drawarea"));
    CHECK(drawarea)
        color1 = GTK_WIDGET(gtk_builder_get_object(builder, "color1"));
    CHECK(color1)
        pen = GTK_BUTTON(gtk_builder_get_object(builder, "pen"));
    CHECK(pen)
        load = GTK_WIDGET(gtk_builder_get_object(builder, "load"));
    CHECK(load)
        erase = GTK_BUTTON(gtk_builder_get_object(builder, "erase"));
    CHECK(erase)
        web = GTK_WIDGET(gtk_builder_get_object(builder, "web"));
    CHECK(web)
        menu = GTK_WIDGET(gtk_builder_get_object(builder, "menu"));
    CHECK(menu)

        grid = GTK_PANED(gtk_builder_get_object(builder, "grid"));
    CHECK(grid)
        grid2 = GTK_PANED(gtk_builder_get_object(builder, "grid2"));
    CHECK(grid2)

    save = GTK_WIDGET(gtk_builder_get_object(builder, "save"));
    CHECK(save)
        retour = GTK_BUTTON(gtk_builder_get_object(builder, "return"));
    CHECK(retour)
        annul = GTK_BUTTON(gtk_builder_get_object(builder, "cancel"));
    CHECK(annul)
        bucket = GTK_BUTTON(gtk_builder_get_object(builder, "bucket"));
    CHECK(bucket)
        rect =  GTK_BUTTON(gtk_builder_get_object(builder, "rectangle"));
    CHECK(rect)
        losange =  GTK_BUTTON(gtk_builder_get_object(builder, "losange"));
    CHECK(losange)
        circle =  GTK_BUTTON(gtk_builder_get_object(builder, "circle"));
    CHECK(circle)

        new = GTK_WIDGET(gtk_builder_get_object(builder, "new"));
    CHECK(new)
        //FILTERS
        filter1 = GTK_WIDGET(gtk_builder_get_object(builder, "filter1"));
    CHECK(filter1)
        filter2 = GTK_WIDGET(gtk_builder_get_object(builder, "filter2"));
    CHECK(filter2)
        filter3 = GTK_WIDGET(gtk_builder_get_object(builder, "filter3"));
    CHECK(filter3)
        filter4 = GTK_WIDGET(gtk_builder_get_object(builder, "filter4"));
    CHECK(filter4)
        filter5 = GTK_WIDGET(gtk_builder_get_object(builder, "filter5"));
    CHECK(filter5)
        filter6 = GTK_WIDGET(gtk_builder_get_object(builder, "filter6"));
    CHECK(filter6)
        filter7 = GTK_WIDGET(gtk_builder_get_object(builder, "filter7"));
    CHECK(filter7)


    GtkWidget *imageBucket = gtk_image_new_from_file("icons/fill.png");
    gtk_button_set_image(bucket, imageBucket);

    GtkWidget *imagePen = gtk_image_new_from_file("icons/pencil.png");
    gtk_button_set_image(pen, imagePen);

    GtkWidget *imageErase = gtk_image_new_from_file("icons/eraser.png");
    gtk_button_set_image(erase, imageErase);

    GtkWidget *imageRectangle = gtk_image_new_from_file("icons/rect.png");
    gtk_button_set_image(rect, imageRectangle);

    GtkWidget *imageCircle = gtk_image_new_from_file("icons/circle.png");
    gtk_button_set_image(circle, imageCircle);
    
    GtkWidget *imageTriangle = gtk_image_new_from_file("icons/triangle.png");
    gtk_button_set_image(triangle, imageTriangle);

    GtkWidget *imageLosange = gtk_image_new_from_file("icons/losange.png");
    gtk_button_set_image(losange, imageLosange);
    
    hscale = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, GTK_ADJUSTMENT(adjustement));
    gtk_container_add(GTK_CONTAINER(grid), hscale);

    hscale2 = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, GTK_ADJUSTMENT(adjustement2));
    gtk_container_add(GTK_CONTAINER(grid2), hscale2);

    //g_signal_connect(G_OBJECT(drawarea), "draw",G_CALLBACK(on_draw_event), NULL); 

    gtk_widget_add_events(drawarea, 
            GDK_BUTTON_PRESS_MASK |
            GDK_BUTTON_MOTION_MASK |
            GDK_BUTTON_RELEASE_MASK |
            GDK_KEY_PRESS_MASK);
    gtk_widget_add_events(gtk_widget_get_toplevel (drawarea), 
            GDK_BUTTON_MOTION_MASK | GDK_BUTTON_PRESS_MASK |
            GDK_BUTTON_MOTION_MASK |
            GDK_BUTTON_RELEASE_MASK );
    g_signal_connect(load, "activate", G_CALLBACK(openfile), NULL);
    g_signal_connect(pen, "clicked", G_CALLBACK(return_draw), NULL);
    g_signal_connect(erase, "clicked", G_CALLBACK(erase_white), NULL);
    g_signal_connect(bucket, "clicked", G_CALLBACK(flood_fill), NULL);
    g_signal_connect(rect, "clicked", G_CALLBACK(get_rect), NULL);
    g_signal_connect(losange, "clicked", G_CALLBACK(get_losange), NULL);
    g_signal_connect(circle, "clicked", G_CALLBACK(get_circle), NULL);
    g_signal_connect(triangle, "clicked", G_CALLBACK(get_triangle), NULL);
    g_signal_connect(drawarea, "button-press-event", G_CALLBACK(on_click), NULL); //blc
    g_signal_connect(drawarea, "motion-notify-event", G_CALLBACK(on_click), NULL); //important
    g_signal_connect(drawarea, "button-release-event", G_CALLBACK(on_click_release), NULL);
    g_signal_connect(color1, "color-set", G_CALLBACK(on_color1_color_set), NULL);
    g_signal_connect(G_OBJECT(drawarea), "draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(web, "activate", G_CALLBACK(website_button), NULL);
    g_signal_connect(save, "activate", G_CALLBACK(on_save),NULL);
    g_signal_connect(hscale, "value-changed", G_CALLBACK(value_changed), NULL);
    g_signal_connect(hscale2, "value-changed", G_CALLBACK(value_changed2), NULL);
    g_signal_connect(retour, "clicked", G_CALLBACK(ctrl_z), NULL);
    g_signal_connect(annul, "clicked", G_CALLBACK(ctrl_y), NULL);
    g_signal_connect(window, "motion-notify-event", G_CALLBACK(refresh), NULL);
    //g_signal_connect(drawarea, "button-release-event", G_CALLBACK(refresh), NULL);
    g_signal_connect(gtk_widget_get_toplevel(menu), "button-release-event", G_CALLBACK(refresh), NULL);
    
    g_signal_connect(new, "activate", G_CALLBACK(loadblank), NULL);
    //FILTERS
    g_signal_connect(filter1, "activate", G_CALLBACK(grey), NULL);
    g_signal_connect(filter2, "activate", G_CALLBACK(invert), NULL);
    g_signal_connect(filter3, "activate", G_CALLBACK(cyann), NULL);
    g_signal_connect(filter4, "activate", G_CALLBACK(rosee), NULL);
    g_signal_connect(filter5, "activate", G_CALLBACK(yelloww), NULL);
    g_signal_connect(filter6, "activate", G_CALLBACK(BAW), NULL);
    g_signal_connect(filter7, "activate", G_CALLBACK(mirrorr), NULL);

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
