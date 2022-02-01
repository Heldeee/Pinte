#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include <gtk/gtk.h>

//#include "display.h"

gchar* filename = "";

void on_show(GtkButton *button, gpointer user_data)
{
    button = button;
    if (app->image_surface == NULL)
    {
	GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nNo image.\n\nPlease, load an image before show.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
	gtk_widget_destroy(dialog);
    }
    else
    {
        GtkBuilder* builder = gtk_builder_new();
        GError* error = NULL;
        if (gtk_builder_add_from_file(builder, "../data/show_window.glade", &error) == 0)
        {
            g_printerr("Error loading file: %s\n", error->message);
            g_clear_error(&error);
        }
        else
        {
	    SDL_SaveBMP(app->image_surface, "../Image/tmp_img/vrai_image.bmp");
	    GtkWindow* w = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
            GtkImage* img = GTK_IMAGE(gtk_builder_get_object(builder, "img"));

            gtk_widget_show_all(GTK_WIDGET(w));
	    gtk_image_set_from_file(img, "../Image/tmp_img/vrai_image.bmp");
            g_signal_connect_swapped(G_OBJECT(w), "destroy", G_CALLBACK(close_window), NULL);
        }
    }
}

/*SDL_Surface* resize(SDL_Surface *img)
{
    double zoomx = (double)nw / (double)w;
    double zoomy = (double)nh / (double)h;
    img = zoomSurface(img, zoomx, zoomy, 0);

    while (img->w > 740 || img->h > 700)
    {
        img = rotozoomSurface(img, 0, 0.9, 0);
    }
    return img;
}*/

void openfile(GtkButton *button, gpointer user_data)
{
    button = button;
    g_print("load\n");

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

void close_window(GtkWidget *widget, gpointer w)
{
    widget = widget;
    gtk_widget_destroy(GTK_WIDGET(w));
}

void on_save(GtkButton *button, gpointer user_data)
{
    button = button;
    
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
            g_print("%s\n", filename);
            break;
        }
        default:
            break;
        }
        gtk_widget_destroy(dialog);
    
}
