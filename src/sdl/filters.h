#ifndef FILTERS_H
#define FILTERS_H


Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Foramt, int value);
void insertionSort(Uint8 arr[], int n);
Uint32 MedianValue(SDL_Surface *image, int x, int y);
void MedianFilter(SDL_Surface *image);
void Flip(SDL_Surface *img);
Uint32 Toblack(Uint32 Pixel, SDL_PixelFormat *Format, int value);



#endif
