#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
//#include "SDL/SDL.h"

//#include "SDL/SDL_image.h"

/*void init_sdl()
{
	// Init only the video part.
	// If it fails, die with an error message.
	
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}*/

/*SDL_Surface* load_image(char *path)
{
	SDL_Surface *img;

	// Load an image using SDL_image with format detection.
	// If it fails, die with an error message.
	img = IMG_Load(path);
	if (!img)
	    errx(3, "can't load %s: %s", path, IMG_GetError());

	return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
	SDL_Surface *screen;

	// Set the window to the same size as the image
	screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
	if (screen == NULL)
	{
	    // error management
	    errx(1, "Couldn't set %dx%d video mode: %s\n",
		    img->w, img->h, SDL_GetError());
	}

	// Blit onto the screen surface
	if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
	    warnx("BlitSurface error: %s\n", SDL_GetError());

	// Update the screen
	SDL_UpdateRect(screen, 0, 0, img->w, img->h);

	// return the screen for further uses
	return screen;
}

void wait_for_keypressed()
{
	SDL_Event event;

	// Wait for a key to be down.
	do
	{
	    SDL_PollEvent(&event);
	} while(event.type != SDL_KEYDOWN);

	// Wait for a key to be up.
	do
	{
	    SDL_PollEvent(&event);
	} while(event.type != SDL_KEYUP);
}

void SDL_FreeSurface(SDL_Surface *surface);*/

#include <SDL2/SDL.h>
//#include <stdio.h>

#define WIDTH 500
#define HEIGHT 500

SDL_Window * window;
SDL_Renderer* renderer;

int running;
int leftButton;
int rightButton;

int mouseX, mouseY;
int previousMouseX, previousMouseY;

int init();
void quit();

void clear()
{
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	SDL_RenderClear(renderer);
}

void setButtonState(Uint8 button,int state)
{
	switch(button)
	{
	case SDL_BUTTON_LEFT:
		leftButton = state;
		break;

	case SDL_BUTTON_RIGHT:
		rightButton= state;
		break;
	}
}

void render()
{
	previousMouseX = mouseX, previousMouseY = mouseY;
	SDL_GetMouseState(&mouseX,&mouseY);
	
	if (leftButton == 0)
	{
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderDrawPoint(renderer,mouseX,mouseY);
		SDL_RenderDrawLine(renderer,previousMouseX,previousMouseY, mouseX,mouseY);
	}
	else if(rightButton == 0)
	{
		SDL_SetRenderDrawColor(renderer,255,255,255,255);
		SDL_RenderDrawPoint(renderer,mouseX,mouseY);
		SDL_RenderDrawLine(renderer,previousMouseX,previousMouseY, mouseX,mouseY);
	}

	SDL_RenderPresent(renderer);
	
	
}

void handleEvent(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_QUIT:
		running = -1;
		break;
	
	case SDL_MOUSEBUTTONDOWN:
		setButtonState(event.button.button,0);
		render();
		break;

	case SDL_MOUSEBUTTONUP:
		setButtonState(event.button.button,-1);
		render();
		break;
	
	case SDL_MOUSEMOTION:
		render();
	}
}
void loop()
{
	while(running == 0)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			handleEvent(event);
		}
	}
}

void quit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
int main()
{
	int return_code;
	return_code = SDL_Init(SDL_INIT_VIDEO);
  	if(return_code < 0) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Surface *surface;
	// 0 means any height
	int bits_per_pixel = 0; // 0 means use the current bpp
	Uint32 flags = SDL_RESIZABLE; // make a resizable window
	surface = SDL_SetVideoMode(WIDTH, HEIGHT, bits_per_pixel, flags);
	if(NULL == surface) {
		fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
		exit(2);
	}
	running = 0;
	leftButton = -1;
	rightButton = -1;

	clear();
	loop();
	quit();
	return 0;
}

