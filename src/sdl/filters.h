#ifndef FILTERS_H_
#define FILTERS_H_

#include <stdlib.h>
#include <SDL/SDL.h>

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void insertionSort(Uint8 arr[], int n);
Uint32 MedianValue(SDL_Surface *image, int x, int y);
void MedianFilter(SDL_Surface* image);
void Flip(SDL_Surface *img);


#endif
