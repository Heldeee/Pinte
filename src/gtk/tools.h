#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <gtk/gtk.h>

void openfile(GtkButton *button, gpointer user_data);
SDL_Surface* resize(SDL_Surface *img);
void close_window(GtkWidget *widget, gpointer w);
void on_save(GtkButton *button, gpointer user_data);

#endif
