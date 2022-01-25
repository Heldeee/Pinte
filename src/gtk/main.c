#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <gtk/gtk.h>

#include "tools.h"

/*typedef struct UserInterface
{
    GtkWindow *window;
    GtkButton *load;
    GtkButton *show;
    GtkButton *save;
    GtkButton *resolve;
    GtkButton *network;
    GtkEntry *Rotation;
    GtkButton *Enter;
    GtkCheckButton *Auto;
    GtkCheckButton *Manual;
    GtkCheckButton *IA;
    GtkCheckButton *bw;
    GtkCheckButton *Grid;
    GtkButton *Generate;
    GtkButton *web;
}UserInterface;

typedef struct Create_sudoku
{
    char *file;
    int posx;
    int posy;
    int grid[81];
    int gridx;
    int gridy;
    SDL_Surface* surface;
    GtkWindow *win;
    GtkImage* img;
    GtkButton *new;
    GtkButton *back;
    GtkButton *add;
    GtkButton *_0;
    GtkButton *_1;
    GtkButton *_2;
    GtkButton *_3;
    GtkButton *_4;
    GtkButton *_5;
    GtkButton *_6;
    GtkButton *_7;
    GtkButton *_8;
    GtkButton *_9;
}cre_sud;

typedef struct Image
{
    GtkImage *img;
    SDL_Surface *rot_img;
    SDL_Surface *otsu_img;
    SDL_Surface *hough_img;
    SDL_Surface *cases_img;
}Image;

typedef struct Application
{
    int is_hough;
    gchar* filename;
    int is_rot;
    int is_resolve;
    int is_otsu;
    int is_generate;
    SDL_Surface* image_surface;
    SDL_Surface* dis_img;

    Network network;
    GtkWindow *pro_w;
    GtkWindow *training_w;
    Image image;
    UserInterface ui;
    cre_sud sud;
}App;*/


int main ()
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Initializes SDL
    //init_sdl();

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "../data/main_window.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "The Ambitions"));
    GtkButton* load = GTK_BUTTON(gtk_builder_get_object(builder, "load"));
    GtkButton* show = GTK_BUTTON(gtk_builder_get_object(builder, "show"));
    GtkButton* save = GTK_BUTTON(gtk_builder_get_object(builder, "save"));
    GtkButton* resolve = GTK_BUTTON(gtk_builder_get_object(builder, "resolve"));
    GtkButton* network = GTK_BUTTON(gtk_builder_get_object(builder, "network"));
    GtkCheckButton* Auto = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Auto"));
    GtkCheckButton* Manual = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Manual"));
    GtkCheckButton* IA = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "IA"));
    GtkCheckButton* bw = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "bw"));
    GtkCheckButton* Grid = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Grid"));
    GtkImage *img = GTK_IMAGE(gtk_builder_get_object(builder, "img_sud"));
    GtkButton* generate = GTK_BUTTON(gtk_builder_get_object(builder, "generate"));
    GtkButton* web = GTK_BUTTON(gtk_builder_get_object(builder, "web"));
    GtkEntry* Rotation = GTK_ENTRY(gtk_builder_get_object(builder,"Rotation"));
    GtkButton* Enter = GTK_BUTTON(gtk_builder_get_object(builder, "Enter"));

    /*App app =
    {
        .filename = "",
	.is_rot = 0,
	.is_resolve = 0,
	.is_otsu = 0,
	.is_generate = 0,
        .pro_w = NULL,
        .image_surface = NULL,
        .dis_img = NULL,
        .training_w = NULL,
        .is_hough = 0,
        .image = 
	{ 
	    .img = img,
	    .rot_img = NULL,
	    .otsu_img = NULL,
	    .hough_img = NULL,
	    .cases_img = NULL,
	},
        .ui =
        {
            .window = window,
            .load = load,
	    .show = show,
            .save = save,
            .resolve = resolve,
            .network = network,
            .Rotation = Rotation,
            .Enter = Enter,
            .Auto = Auto,
            .Manual = Manual,
            .IA = IA,
            .bw = bw,
            .Grid = Grid,
	    .Generate = generate,
	    .web = web,
        },
    {*/

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Auto), TRUE);

    Auto = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Auto"));

    gtk_widget_set_sensitive(GTK_WIDGET(Rotation), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(IA), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(bw), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(Grid), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(Enter), FALSE);

    // Connects signal handlers.
    //g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(load, "clicked", G_CALLBACK(openfile), NULL);
    //g_signal_connect(show, "clicked", G_CALLBACK(on_show), &app);
    g_signal_connect(save, "clicked", G_CALLBACK(on_save), NULL);
    //g_signal_connect(bw, "clicked", G_CALLBACK(BlackWhite), &app);
    //g_signal_connect(web, "clicked", G_CALLBACK(open_website), &app);
    //g_signal_connect(Enter,"clicked",G_CALLBACK(value_changed), &app);

    // Runs the main loop.
    gtk_main();

    /*SDL_FreeSurface(app.image_surface);
    SDL_FreeSurface(app.dis_img);
    SDL_FreeSurface(app.image.otsu_img);
    SDL_FreeSurface(app.image.rot_img);*/

    return 0;
}
