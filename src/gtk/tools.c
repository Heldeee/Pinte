#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include <gtk/gtk.h>

gchar* filename = "";
/*void display_pro(App* app)
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
}*/

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
            //g_print("%s\n", filename);
            break;
        }
        default:
            break;
        }
        gtk_widget_destroy(dialog);
    
}
