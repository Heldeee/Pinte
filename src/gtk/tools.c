#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include <gtk/gtk.h>


void display_pro(App* app)
{
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "../data/progress.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
    }
    else
    {
        GtkWindow* pro_w = GTK_WINDOW(gtk_builder_get_object(builder, "pro_w"));
        GtkProgressBar* pro_bar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "pro_bar"));
        GtkLabel *label = GTK_LABEL(gtk_builder_get_object(builder, "label"));

        gtk_label_set_label(label, "Resolving...\n");
    app->pro_w = pro_w;
        g_timeout_add(1000, (GSourceFunc)des_w, app);
        g_timeout_add(1000, (GSourceFunc)handle_progress, pro_bar);
        gtk_widget_show_all(GTK_WIDGET(pro_w));
        g_signal_connect_swapped(G_OBJECT(pro_w), "destroy", G_CALLBACK(close_window), NULL);
    }
}

SDL_Surface* resize(SDL_Surface *img)
{
    /*double zoomx = (double)nw / (double)w;
    double zoomy = (double)nh / (double)h;
    img = zoomSurface(img, zoomx, zoomy, 0);*/

    while (img->w > 740 || img->h > 700)
    {
        img = rotozoomSurface(img, 0, 0.9, 0);
    }
    return img;
}

void openfile(GtkButton *button, gpointer user_data)
{
    button = button;
    g_print("load\n");

    App* app = user_data;

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
        app->filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	app->image_surface = load_image(app->filename);
        app->dis_img = load_image(app->filename);
        //g_print("Weight = %i\n", app->image_surface->w);
        //g_print("Height = %i\n", app->image_surface->h);
        app->dis_img = resize(app->dis_img);
        SDL_SaveBMP(app->dis_img, "../Image/tmp_img/display.bmp");
        //g_print("Weight = %i\n", app->dis_img->w);
        //g_print("Height = %i\n", app->dis_img->h);
        gtk_image_set_from_file(app->image.img, "../Image/tmp_img/display.bmp");
        app->is_resolve = 0;
	app->is_otsu = 0;
	app->is_rot = 0;
	app->is_generate = 0;
	app->is_hough = 0;
	app->is_training = 0;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.bw), FALSE);
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
    App* app = user_data;
    if (app->image_surface == NULL)
    {
	GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nNo image.\n\nPlease, load an image before save.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
	gtk_widget_destroy(dialog);
    }
    else if (app->is_resolve == 0)
    {
        GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nSudoku is not resolved.\n\nPlease, resolve the sudoku before save.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
	gtk_widget_destroy(dialog);
    }
    else
    {
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
            //g_print("%s\n", filename);
            SDL_Surface* s = load_image("../Image/tmp_img/result.bmp");
            SDL_SaveBMP(s, filename);
            break;
        }
        default:
            break;
        }
        gtk_widget_destroy(dialog);
    }
}
