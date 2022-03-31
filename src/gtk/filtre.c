#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include "pixel_operations.c"
#include <math.h>

void init_sdl()
{
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

void SDL_FreeSurface(SDL_Surface *surface);

void __negative(SDL_Surface *surf)
{
    int height = surf -> h;
    int width = surf -> w;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            Uint32 pixel = get_pixel(surf, x, y);
            Uint8 r;
            Uint8 g;
            Uint8 b;
            SDL_GetRGB(pixel, surf->format, &r, &g, &b);
            Uint32 new_pixel = SDL_MapRGB(surf->format, 255 -r, 255 - g, 255 - b);
            put_pixel(surf, x, y, new_pixel);
        }
    }
}

int negative(char path[])
{
  char result[] = "./Results/negative.bmp";
  
  init_sdl();
  SDL_Surface *image = load_image(path);

  __negative(image);
      
  SDL_SaveBMP(image, result);
  SDL_FreeSurface(image);
    
  return 0;
}


void __mirror(SDL_Surface *surf)
{
    int height = surf -> h;
    int width = surf -> w;

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width/2; x++)
        {
            Uint32 pixel_left = get_pixel(surf, x, y);
	    Uint32 pixel_right = get_pixel(surf, width-1-x, y);
	    
            put_pixel(surf, width-1-x, y, pixel_left);
	    put_pixel(surf, x, y, pixel_right); 
        }
    }
}

int mirror(char path[])
{
  char result[] = "./Results/mirror.bmp";
  
  init_sdl();
  SDL_Surface *image = load_image(path);

  __mirror(image);
      
  SDL_SaveBMP(image, result);
  SDL_FreeSurface(image);
    
  return 0;
}



void __grayscale(SDL_Surface *surf)
{
    int height = surf -> h;
    int width = surf -> w;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            Uint32 pixel = get_pixel(surf, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surf->format, &r, &g, &b);
            Uint8 grey = 0.3 * r + 0.59 * g + 0.11 * b;
            Uint32 new_pixel = SDL_MapRGB(surf->format, grey, grey, grey);
            put_pixel(surf, x, y, new_pixel);
        }
    }
}

int grayscale(char path[])
{
  char result[] = "./Results/grayscale.bmp";
      
  init_sdl();
  SDL_Surface *image = load_image(path);
      
  __grayscale(image);
      
  SDL_SaveBMP(image, result);
  SDL_FreeSurface(image);
    
  return 0;
}



void __yellow(SDL_Surface *surf)
{
    int height = surf -> h;
    int width = surf -> w;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            Uint32 pixel = get_pixel(surf, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surf->format, &r, &g, &b);
            Uint32 new_pixel = SDL_MapRGB(surf->format, r, g, 0);
            put_pixel(surf, x, y, new_pixel);
        }
    }
}

int yellow(char path[])
{
  char result[] = "./Results/yellow.bmp";
      
  init_sdl();
  SDL_Surface *image = load_image(path);
      
  __yellow(image);
      
  SDL_SaveBMP(image, result);
  SDL_FreeSurface(image);
    
  return 0;
}


void __rose(SDL_Surface *surf)
{
    int height = surf -> h;
    int width = surf -> w;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            Uint32 pixel = get_pixel(surf, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surf->format, &r, &g, &b);
            Uint32 new_pixel = SDL_MapRGB(surf->format, r, 0, b);
            put_pixel(surf, x, y, new_pixel);
        }
    }
}

int rose(char path[])
{
  char result[] = "./Results/rose.bmp";
      
  init_sdl();
  SDL_Surface *image = load_image(path);
      
  __rose(image);
      
  SDL_SaveBMP(image, result);
  SDL_FreeSurface(image);
    
  return 0;
}


void __cyan(SDL_Surface *surf)
{
    int height = surf -> h;
    int width = surf -> w;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            Uint32 pixel = get_pixel(surf, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surf->format, &r, &g, &b);
            Uint32 new_pixel = SDL_MapRGB(surf->format, 0, g, b);
            put_pixel(surf, x, y, new_pixel);
        }
    }
}

int cyan(char path[])
{
  char result[] = "./Results/cyan.bmp";
      
  init_sdl();
  SDL_Surface *image = load_image(path);
      
  __cyan(image);
      
  SDL_SaveBMP(image, result);
  SDL_FreeSurface(image);
    
  return 0;
}


void __blackandwhite(SDL_Surface *surf)
{
    int height = surf -> h;
    int width = surf -> w;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
	  {
	    Uint32 pixel = get_pixel(surf, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surf->format, &r, &g, &b);
            Uint8 grey = 0.3 * r + 0.59 * g + 0.11 * b;
	    Uint32 new_pixel;
	    if (grey > 127)
	      {
	        new_pixel = SDL_MapRGB(surf->format, 0, 0, 0);
	      }
	    else
	      {
	        new_pixel = SDL_MapRGB(surf->format, 255, 255, 255);
	      }
            put_pixel(surf, x, y, new_pixel);
	  }
    }
}


int blackandwhite(char path[])
{
  char result[] = "./Results/blackandwhite.bmp";
  
  init_sdl();
  SDL_Surface *image = load_image(path);
      
  __blackandwhite(image);
      
  SDL_SaveBMP(image, result);
  SDL_FreeSurface(image);
    
  return 0;
}
