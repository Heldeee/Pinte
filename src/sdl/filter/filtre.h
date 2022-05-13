#ifndef PIXEL_OPERATIONS_H_
#define PIXEL_OPERATIONS_H_

#include <stdlib.h>
#include <SDL.h>

void init_sdl();
SDL_Surface* load_image(char *path);
void SDL_FreeSurface(SDL_Surface *surface);

void __negative(SDL_Surface *surf);
int negative(char path[]);

void __mirror(SDL_Surface *surf);
int mirror(char path[]);

void __grayscale(SDL_Surface *surf);
int grayscale(char path[]);

void __yellow(SDL_Surface *surf);
int yellow(char path[]);

void __rose(SDL_Surface *surf);
int rose(char path[]);

void __cyan(SDL_Surface *surf);
int cyan(char path[]);

void __blackandwhite(SDL_Surface *surf);
int blackandwhite(char path[]);

SDL_Surface* SDL_RotationCentralN(SDL_Surface* origine, float angle);
void rotat(char path[]);

SDL_Surface* __rotationr(SDL_Surface *surf);
int rotationr(char path[]);


SDL_Surface* __rotationl(SDL_Surface *surf);
int rotationl(char path[]);

#endif
