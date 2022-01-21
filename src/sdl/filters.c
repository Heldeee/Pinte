#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <float.h>

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

// get the pixel data depending on the format used

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

// Put the given pixel in the surface at the coordinates (x,y)
// Adapting to the format of the image

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

// Blackscale function
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format, int value)
{
    if(value !=3)
    {
    }
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(Pixel, Format, &r, &g, &b);
    if (r<=240 || g<=240 || b<=240)
    {
        return SDL_MapRGB(Format, 255, 255, 255);
    }
    else
    {
        return SDL_MapRGB(Format, 0, 0, 0);
    }
}

void insertionSort(Uint8 arr[], int n)
{
    int i, j;
    Uint8 key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
 
        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

Uint32 MedianValue(SDL_Surface *image, int x, int y)
{
    Uint32 pix[9];
    Uint8 tab[9];
    Uint8 r;
    Uint8 g;
    Uint8 b;

    pix[0] = get_pixel(image, x-1, y+1);
    pix[1] = get_pixel(image, x, y+1);
    pix[2] = get_pixel(image, x+1, y+1);
    pix[3] = get_pixel(image, x-1, y);
    pix[4] = get_pixel(image, x, y);
    pix[5] = get_pixel(image, x+1, y);
    pix[6] = get_pixel(image, x-1, y-1);
    pix[7] = get_pixel(image, x, y-1);
    pix[8] = get_pixel(image, x+1, y-1);

    for (int i = 0; i < 9; i++)
    {
        SDL_GetRGB(pix[i], image->format, &r,&g,&b);
        tab[i] = r;
    }
    
    insertionSort(tab, 9);
    int mid = 0;
    for (int j = 2; j <= 6; j++)
        mid += (int) tab[j];
    mid = mid/5;
    return SDL_MapRGB(image->format, tab[4],tab[4],tab[4]);
    //return SDL_MapRGB(image->format, mid,mid,mid);
}

void MedianFilter(SDL_Surface* image)
{
    for (int y = 1; y < image->h-1; y++)
    {
        for (int x = 1; x < image->w-1; x++)
            put_pixel(image, x, y, MedianValue(image, x, y));
    }
}


void Flip(SDL_Surface *img)
{
    SDL_Surface *flipped = SDL_CreateRGBSurface(0, img -> h, img -> w, 32, 0, 0, 0, 0);

    Uint32 pixel;

    for (int i = 0; i < img -> h; i++)
    {
        for (int j = 0; j < img -> w; j++)
        {
            pixel = get_pixel(img, j, i);
            put_pixel(flipped, i, j, pixel);
        }
    }

    *img = *flipped;

    free(flipped);
}

Uint32 Toblack(Uint32 Pixel, SDL_PixelFormat *Format, int value)
{
    if(value!=3)
    {
    }
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(Pixel, Format, &r, &g, &b);
    if (r>=240 || g>=240 || b>=240)
    {
        return SDL_MapRGB(Format, 0, 0, 0);
    }
    else
    {
        return SDL_MapRGB(Format, 255, 255, 255);
    }
}







