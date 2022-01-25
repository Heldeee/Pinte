#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include <gtk/gtk.h>

#include "tools.h"

int main ()
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Initializes SDL
    init_sdl();

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

    App app =
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
	.network =
	{
	    .inputsize = SIZE,
	    .hiddensize = HID,
	    .outputsize = OUT,
	    .input = {},
	    .values = {},
	    .output = {},
	    .b1 = {},
	    .b2 = {},
	    .w1 = {},
	    .w2 = {}, 
	},
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
	.sud =
	{
	    .file = "",
	    .posx = 4,
	    .posy = 4,
	    .grid = {},
	    .gridx = 0,
	    .gridy = 0,
	    .surface = NULL,
	    .img = NULL,
	    .win = NULL,
	    .new = NULL,
	    .add = NULL,
	    .back = NULL,
	    ._0 = NULL,
	    ._1 = NULL,
	    ._2 = NULL,
	    ._3 = NULL,
	    ._4 = NULL,
	    ._5 = NULL,
	    ._6 = NULL,
	    ._7 = NULL,
	    ._8 = NULL,
	    ._9 = NULL,
	}
    };

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Auto), TRUE);

    Auto = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Auto"));

    gtk_widget_set_sensitive(GTK_WIDGET(Rotation), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(IA), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(bw), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(Grid), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(Enter), FALSE);

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(load, "clicked", G_CALLBACK(openfile), &app);
    g_signal_connect(show, "clicked", G_CALLBACK(on_show), &app);
    g_signal_connect(save, "clicked", G_CALLBACK(on_save), &app);
    g_signal_connect(resolve, "clicked", G_CALLBACK(on_resolve), &app);
    g_signal_connect(network, "clicked", G_CALLBACK(on_network), &app);
    g_signal_connect(Manual, "clicked", G_CALLBACK(Manu), &app);
    g_signal_connect(Auto, "clicked", G_CALLBACK(Automatic), &app);
    g_signal_connect(IA, "clicked", G_CALLBACK(IA_recognition), &app);
    g_signal_connect(bw, "clicked", G_CALLBACK(BlackWhite), &app);
    g_signal_connect(Grid, "clicked", G_CALLBACK(GridDetec), &app);
    g_signal_connect(generate, "clicked", G_CALLBACK(generate_sud), &app);
    g_signal_connect(web, "clicked", G_CALLBACK(open_website), &app);
    g_signal_connect(Enter,"clicked",G_CALLBACK(value_changed), &app);

    // Runs the main loop.
    gtk_main();

    SDL_FreeSurface(app.image_surface);
    SDL_FreeSurface(app.dis_img);
    SDL_FreeSurface(app.image.otsu_img);
    SDL_FreeSurface(app.image.rot_img);

    return 0;
}
