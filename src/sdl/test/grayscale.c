#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

void init_sdl()
{
	// Init only the video part.
	// If it fails, die with an error message.
	
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
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

void SDL_FreeSurface(SDL_Surface *surface);

void array_swap(int array[], size_t i, size_t j)
{
  int element1 = array[i];
  int element2 = array[j];
  array[i] = element2;
  array[j] = element1;
}

void array_select_sort(int array[], size_t len)
{
  size_t i = 0;
  size_t j;
  int min_index;
  while(i<len)
  {
    /* Find Min Index */
    j= i;
    min_index = j;
    while(j<len)
    {
        if(array[j]<array[min_index])
        {
            min_index = j;
        }
        j+=1;
    }
    array_swap(array,i,min_index);
    i++;
  }
}

void noiseReduction(SDL_Surface *img)
{
  /* Variables */
  int w;
  int h;
  w = img -> w;
  h = img -> h;
  int pixelTable[5];

 for(int i = 0; i < w; i++)
 {
   for(int j = 0; j < h; j++)
   {
     for(int k = j; k <= j + 4; k++)
     {
       //Borders of picture
       if(i == 0)
       {
         if(k == 0)
         {
            pixelTable[0] = get_pixel(img, i, k);
            pixelTable[1] = get_pixel(img, i, k);
            pixelTable[2] = get_pixel(img, i, k);
            pixelTable[3] = get_pixel(img, i, k + 1);
            pixelTable[4] = get_pixel(img, i + 1, k);
            break;
         }
         if(k == h)
         {
            pixelTable[0] = get_pixel(img, i, k);
            pixelTable[1] = get_pixel(img, i, k - 1);
            pixelTable[2] = get_pixel(img, i, k);
            pixelTable[3] = get_pixel(img, i, k);
            pixelTable[4] = get_pixel(img, i + 1, k);
            break;
         }
         else
         {
          pixelTable[0] = get_pixel(img, i, k);
          pixelTable[1] = get_pixel(img, i, k - 1);
          pixelTable[2] = get_pixel(img, i, k);
          pixelTable[3] = get_pixel(img, i, k + 1);
          pixelTable[4] = get_pixel(img, i + 1, k);
          break;
         }
       }
       if(i == w)
       {
          if(k == 0)
          {
            pixelTable[0] = get_pixel(img, i, k);
            pixelTable[1] = get_pixel(img, i, k);
            pixelTable[2] = get_pixel(img, i - 1, k);
            pixelTable[3] = get_pixel(img, i, k + 1);
            pixelTable[4] = get_pixel(img, i, k);
            break;
          }
          if(k == h)
          {
            pixelTable[0] = get_pixel(img, i, k);
            pixelTable[1] = get_pixel(img, i, k - 1);
            pixelTable[2] = get_pixel(img, i - 1, k);
            pixelTable[3] = get_pixel(img, i, k);
            pixelTable[4] = get_pixel(img, i, k);
            break;
          }
          else
          {
            pixelTable[0] = get_pixel(img, i, k);
            pixelTable[1] = get_pixel(img, i, k - 1);
            pixelTable[2] = get_pixel(img, i - 1, k);
            pixelTable[3] = get_pixel(img, i, k + 1);
            pixelTable[4] = get_pixel(img, i, k);
            break;
          }
       }
       if(k == 0)
       {
          pixelTable[0] = get_pixel(img, i, k);
          pixelTable[1] = get_pixel(img, i, k);
          pixelTable[2] = get_pixel(img, i - 1, k);
          pixelTable[3] = get_pixel(img, i, k + 1);
          pixelTable[4] = get_pixel(img, i + 1, k);
          break;
       }
       if(k == h)
       {
        pixelTable[0] = get_pixel(img, i, k);
        pixelTable[1] = get_pixel(img, i, k - 1);
        pixelTable[2] = get_pixel(img, i - 1, k);
        pixelTable[3] = get_pixel(img, i, k);
        pixelTable[4] = get_pixel(img, i + 1, k);
        break;
       }
       else
       {
        pixelTable[0] = get_pixel(img, i, k);
        pixelTable[1] = get_pixel(img, i, k - 1);
        pixelTable[2] = get_pixel(img, i - 1, k);
        pixelTable[3] = get_pixel(img, i, k + 1);
        pixelTable[4] = get_pixel(img, i + 1, k);
        break;
       }
     }
      array_select_sort(pixelTable, 5);
      int med = pixelTable[2];
      put_pixel(img, i, j, med);
   }
 }
}

SDL_Surface* Filter(SDL_Surface* source)
{
	SDL_Surface *target;
	int x, y;

	if( source->flags & SDL_SRCCOLORKEY )
	{	
		target = SDL_CreateRGBSurface( SDL_SWSURFACE, source->w ,source->h, source->format->BitsPerPixel, source->format->Rmask, source->format->Gmask, source->format->Bmask, 0 );
        }
    	else
        {
        	target = SDL_CreateRGBSurface( SDL_SWSURFACE, source->w ,source->h, source->format->BitsPerPixel, source->format->Rmask, source->format->Gmask, source->format->Bmask, source->format->Amask );
	}

	int w = source->w;
	int h = source->h;
	for(y=0; y<h; y++)
	{
		for(x=0; x<w; x++)
		{
			unsigned r = 0, g = 0, b = 0;
			Uint8 rt, gt, bt;
			if (y != 0) 
			{
			   
			   	unsigned o = get_pixel(source, x, y-1);
				SDL_GetRGB(o, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
				
			}
			

			if (x != 0)
			{
				unsigned d = get_pixel(source, x-1, y);
				SDL_GetRGB(d, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
			}
			
			if ((x != 0) && (y != 0))
			{
				unsigned a = get_pixel(source,x-1, y-1);
				SDL_GetRGB(a, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
			}
			
			if ((x < w-1) && (y !=0))
			{
				
				unsigned c = get_pixel(source, x+1, y-1);
				SDL_GetRGB(c, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
			}
			
			if (x < w-1) 
			{
				unsigned e = get_pixel(source,x+1, y);
				SDL_GetRGB(e, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
			}
			
			if ((x !=0 ) && (y < h+1))
			{
					
				unsigned f = get_pixel(source,x-1, y+1);
				SDL_GetRGB(f, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
			
			}
			
			
			if (y < h-1) 
			{
				unsigned k = get_pixel(source,x  , y+1);
				SDL_GetRGB(k, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
			}
			
						
			if ((x < w+1 ) && (y < h+1))
			{
					
				unsigned h = get_pixel(source,x+1, y+1);
				SDL_GetRGB(h, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
							
			}
			
			unsigned z = get_pixel(source, x, y);
			SDL_GetRGB(z, source->format, &rt, &gt, &bt);
			r += rt;
			g += gt;
			b += bt;
		
		   	
		   	r/=9;
		   	g/=9;
		   	b/=9;
			
			
			unsigned avg = SDL_MapRGB(target->format, r, g, b);	
            put_pixel(target,x, y, avg);
		}
	}
	return target;
}


float compute_first_order_cumulative_moment(float* hist,int k)
{
	float first_order_cumulative_moment = 0;
	for (int i = 0; i < k; i++)
	{
		first_order_cumulative_moment += i*hist[i];

	}

	return first_order_cumulative_moment;
}

float compute_zero_order_cumulative_moment(float* hist,int k)
{
	float zero_order_cumulative_moment = 0;

	for (int i = 0; i < k; i++)
	{

		zero_order_cumulative_moment += i*hist[i];

	}
	return zero_order_cumulative_moment;
}

float compute_variance_class_separability(float uT,float wk, float uk)
{
    return pow((uT*wk-uk),2)/(wk*(1-wk));

}

void otsu(SDL_Surface* img, float seuil)
{
	float hist[256];

	for (int i = 0; i < 256; i++)

	{

		hist[i] = 0;

	}

	int width = img -> w;
	int height = img -> h;
	Uint8 r,g,b;
	for (int i = 0; i < height ; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Uint32 pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &r , &g, &b);
			hist[r] += 1;
		}
	}

	int N = width*height;

	for (int i = 0; i < 256; i++)

	{

		hist[i] = hist[i] / N;

	}

	float w[256],u[256],uT;

	for (int i = 0; i < 256; i++)

	{

		w[i] = compute_zero_order_cumulative_moment(hist, i);

		u[i] = compute_first_order_cumulative_moment(hist, i);

	}

	uT = compute_first_order_cumulative_moment(hist, 256);

	float variance_class_separability_max = -1;

	float best_threesold = 0;

	for (int i = 0; i < 256; i++)
	{
		int vk = compute_variance_class_separability(uT, w[i], u[i]);

		if (vk > variance_class_separability_max)
		{
			variance_class_separability_max = vk;
			best_threesold = i;
		}

	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)

		{
			Uint32 pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &r ,&g,&b);
			if (r < best_threesold * seuil)
			{
				pixel = SDL_MapRGB(img ->format, 0, 0, 0);
				put_pixel(img,j,i,pixel);
			}
			else
			{
				pixel = SDL_MapRGB(img->format, 255, 255, 255);
				put_pixel(img,j,i,pixel);
			}
		}
	}
}

void test(SDL_Surface *img,int ecart)
{
    int h = img->h;
    int w = img->w ;
    int r = 0;
    int T = h*w;
	Uint8 rt, gt, bt;

    for (int i =0; i< h ;i++)
    {
        for (int j= 0;j<h ; j++)
        {

            unsigned pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &rt, &gt, &bt);
			r += rt;
        }
    }

   int avg = r /T;

    for (int i =0; i <h;i++)
    {
        for (int j=0; j<w;j++)
        {
            unsigned pixel = get_pixel(img,j,i);
            SDL_GetRGB(pixel,img->format,&rt,&gt,&bt);
            if (rt < avg + ecart)
            {
                pixel = SDL_MapRGB(img->format, 0,0,0);
                put_pixel(img,j,i,pixel);
            }
        }
    }
}

void ots(SDL_Surface* img)
{
    int width = img -> w;
	int height = img -> h;
	Uint8 r,g,b;

    int hist[256];
    for (int i =0;i<256;i++)
    {
        hist[i]=0;
    }

    for (int i = 0; i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            Uint32 pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &r ,&g,&b);
            hist[r]+=1;

        }
    }

    float ut = 0;
    float E2= 0;
    float N = height*width;

    for (int i = 0;i<256;i++)
    {

        ut += i*(hist[i]/N);
        E2 += i*i*(hist[i]/N); 
    }


    float v= E2 - (ut*ut);
    float ecart = sqrt(v);

    /*if (ut > 197 && ut<205)
    {
        test(img,ecart);
    }*/
    if (ecart<60 && ecart>50)
    {
        ut-=20;
    }

    for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)

		{

            Uint32 pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &r ,&g,&b);

			if (r + ecart < ut)
			{

				pixel = SDL_MapRGB(img ->format, 0, 0, 0);
				put_pixel(img,j,i,pixel);

			}
			else
			{
				pixel = SDL_MapRGB(img->format, 255, 255, 255);
				put_pixel(img,j,i,pixel);
			}
		}
	}
}

void GammaCorrection(SDL_Surface * img,float gamma)
{
    int h = img -> h;
    int w = img ->w ;
    Uint8 r,g,b;
    Uint8 newr,newg,newb;

    for (int i = 0;i < h;i++)
    {
        for (int j=0;j<w;j++)
        {
            float invgamma = 1/gamma;
            Uint32 pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &r ,&g,&b);

            newr = (int) (pow((float)(r)/255.0,invgamma)*255);
            newg = (int) (pow((float)(g)/255.0,invgamma)*255);
            newb = (int) (pow((float)(b)/255.0,invgamma)*255);

             Uint32 newpixel = SDL_MapRGB(img->format,newr,newg,newb);
            put_pixel(img,j,i,newpixel);
        }
    }
}

void ot(SDL_Surface* img)
{
    int width = img -> w;
	int height = img -> h;
	Uint8 r,g,b;
    int total = width * height;

    int hist[256];
    for (int i =0;i<256;i++)
    {
        hist[i]=0;
    }

    for (int i = 0; i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            Uint32 pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &r ,&g,&b);
            hist[r]+=1;

        }
    }

   float ut = 0;
    float E2= 0;
    float N = height*width;
   
    for (int i = 0;i<256;i++)
    {

        ut += i*(hist[i]/N);
        E2 += i*i*(hist[i]/N); 
    }
    
    
    float v= E2 - (ut*ut);
    float ecart = sqrt(v);

    if (ecart>100 && ut <100)
    {

        GammaCorrection(img,0.5);
    }
    if (ecart>55 && v < 3500)
    {

        GammaCorrection(img,4);
    }
    
    for (int i =0;i<256;i++)
    {
        hist[i]=0;
    }

    for (int i = 0; i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            Uint32 pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &r ,&g,&b);
            hist[r]+=1;

        }
    }

   ut = 0;
   E2= 0;
   
    for (int i = 0;i<256;i++)
    {

        ut += i*(hist[i]/N);
        E2 += i*i*(hist[i]/N); 
    }
    v= E2 - (ut*ut);
    ecart = sqrt(v);
   
    
    float sum = 0;
    for (int t = 0; t<256;t++)
    {
        sum+=t * hist[t];
    }

    float sumB=0;
    int wB=0;
    int wF=0;

    float varMax=0;
    int threshold = 0;

    for (int t =0;t<256;t++)
    {
        wB += hist[t];
        if (wB==0)
        {
            continue;
        }

        wF = total - wB;
        if (wF==0)
        {
            break;
        }

        sumB+= (float) (t*hist[t]);

        float mB=sumB/wB;
        float mF = (sum -sumB)/wF;

        float varBetween = (float) wB * (float) wF * (mB -mF) * (mB-mF);

        if (varBetween > varMax)
        {
            varMax = varBetween;
            threshold = t;
        
        }
     }

    int alpha=0;
    if (ecart > 55)
    { 
        alpha = ut-threshold;
    }
    if (ut > 230)
    {    
        alpha = - ecart;
    }
  
        for (int i = 0; i < height; i++)
	    {

		    for (int j = 0; j < width; j++)
		    {

                Uint32 pixel = get_pixel(img, j, i);
			    SDL_GetRGB(pixel, img->format, &r ,&g,&b);

                if (r < threshold - alpha)
                {
                    pixel = SDL_MapRGB(img ->format, 0, 0, 0);
                    put_pixel(img,j,i,pixel);
                }
                else
                {
                    pixel = SDL_MapRGB(img->format, 255, 255, 255);
                    put_pixel(img,j,i,pixel);
                }
		    }
        }        
}

SDL_Surface* kernel (SDL_Surface* image_surface)
{


    Uint8 r, g, b;
    Uint8 r1, g1, b1;
    Uint8 r2, g2, b2;
    Uint8 r3, g3, b3;
    Uint8 r4, g4, b4;
    Uint8 r5, g5, b5;
    Uint8 r6, g6, b6;
    Uint8 r7, g7, b7;
    Uint8 r8, g8, b8;
    Uint8 color;
    Uint32 pixel,pixel1,pixel2,pixel3,pixel4,pixel5,pixel6,pixel7,pixel8;

    Uint32 Blackpixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
    Uint32 Whitepixel = SDL_MapRGB(image_surface->format, 255, 255, 255);

    long width = image_surface -> w;
    long hight = image_surface -> h;

    SDL_Surface* destination = SDL_CreateRGBSurface(SDL_HWSURFACE, width, hight, image_surface->format->BitsPerPixel,
                 image_surface->format->Rmask, image_surface->format->Gmask, image_surface->format->Bmask, image_surface->format->Amask);

    for (long y = 1 ; y < hight - 1; y++)
    {
        for (long x = 1; x < width - 1; x++)
        {
                pixel = get_pixel(image_surface, x, y);
                pixel1 = get_pixel(image_surface, x+1, y);
                pixel2 = get_pixel(image_surface, x+1, y+1);
                pixel3 = get_pixel(image_surface, x+1, y-1);
                pixel4 = get_pixel(image_surface, x, y-1);
                pixel5 = get_pixel(image_surface, x, y+1);
                pixel6 = get_pixel(image_surface, x-1, y-1);
                pixel7 = get_pixel(image_surface, x-1, y);
                pixel8 = get_pixel(image_surface, x-1, y+1);

                SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
                SDL_GetRGB(pixel1, image_surface -> format, &r1, &g1, &b1);
                SDL_GetRGB(pixel2, image_surface -> format, &r2, &g2, &b2);
                SDL_GetRGB(pixel3, image_surface -> format, &r3, &g3, &b3);
                SDL_GetRGB(pixel4, image_surface -> format, &r4, &g4, &b4);
                SDL_GetRGB(pixel5, image_surface -> format, &r5, &g5, &b5);
                SDL_GetRGB(pixel6, image_surface -> format, &r6, &g6, &b6);
                SDL_GetRGB(pixel7, image_surface -> format, &r7, &g7, &b7);
                SDL_GetRGB(pixel8, image_surface -> format, &r8, &g8, &b8);

                color = r -(r1/8 + r2/8 + r3/8 + r4/8 + r5/8 + r6/8 + r7/8 + r8/8);

                if (color > 50)
                {
                        put_pixel(destination, x, y, Whitepixel);
                }
                else
                {
                        put_pixel(destination, x, y, Blackpixel);
                }
         }          
    }
    
    return destination;
}


void SDL_ExitWithError(const char *message);

SDL_Surface* SDL_RotationCentralN(SDL_Surface* origine, float angle)
{
 SDL_Surface* destination;
 int i;
 int j;
 Uint32 couleur;
 int mx, my, mxdest, mydest;
 int bx, by;
 float angle_radian;
 float tcos;
 float tsin;
 double largeurdest;
 double hauteurdest;
 angle_radian = -angle * M_PI / 180.0;

 tcos = cos(angle_radian);
 tsin = sin(angle_radian);

 largeurdest=   ceil(origine->w * fabs(tcos) + origine->h * fabs(tsin)),
 hauteurdest=   ceil( origine->w * fabs(tsin) + origine->h * fabs(tcos)),


 destination = SDL_CreateRGBSurface(SDL_HWSURFACE, largeurdest, hauteurdest, origine->format->BitsPerPixel,
			origine->format->Rmask, origine->format->Gmask, origine->format->Bmask, origine->format->Amask);


 if(destination==NULL)
  return NULL;

 mxdest = destination->w/2.;
 mydest = destination->h/2.;
 mx = origine->w/2.;
 my = origine->h/2.;

 for(j=0;j<destination->h;j++)
  for(i=0;i<destination->w;i++)
  {
   bx = (ceil (tcos * (i-mxdest) + tsin * (j-mydest) + mx));
   by = (ceil (-tsin * (i-mxdest) + tcos * (j-mydest) + my));

   if (bx>=0 && bx< origine->w && by>=0 && by< origine->h)
   {
     couleur = get_pixel(origine, bx, by);
     put_pixel(destination, i, j, couleur);
   }
 }

return destination;
}

int RotationAuto(SDL_Surface* img,float varMax,int anglevarMax)
{
   	int w = img -> w;
	int h = img -> h;
	Uint8 r,g,b;

    int hist[h];
    for (int i =0;i<h;i++)
    {
        hist[i]=0;
    }

    float ut = 0;
    float E2= 0;
    //float N = h*w;
    float varBetween =0;
 

    for (int i = 0; i<h;i++)
    {
        for (int j=0;j<w;j++)
        {
            Uint32 pixel = get_pixel(img, j, i);
		    SDL_GetRGB(pixel, img->format, &r ,&g,&b);
            
            if (r<127)
            {
                hist[i]+=1;   
            }
        }
    }

    for (int i = 0;i<h;i++)
    {
        ut += hist[i];
    }
   
    ut=ut/h;

    for (int i=0;i<h;i++)
    {
        E2+= (hist[i]-ut) * (hist[i]-ut);
    }
    
    E2=E2/h;

    varBetween=sqrt(E2);
    
    printf("%f\n",varBetween);
    printf("%f\n",varMax);

    if(anglevarMax<=45 && anglevarMax>=0)
    {
        if (varBetween > varMax-1)
        {
            varMax = varBetween;
            anglevarMax+=1; 
            img=SDL_RotationCentralN (img, 1);
            return RotationAuto(img,varMax,anglevarMax);
        }

        if (anglevarMax>37)
        {
            return 0; 
        }

    }
    else
    {
        anglevarMax = -anglevarMax;
        img=SDL_RotationCentralN (img, anglevarMax);
    }
 

    return anglevarMax;
    
}




int main(int argc,char *argv[])
{
	
	if (argc != 2)
		errx(1, "must provide filename");
	
	#define filename argv[1]

	
	SDL_Surface* image_surface;
	SDL_Surface* screen_surface;

	// Initialize the SDL
	init_sdl();

	image_surface = load_image(filename);
	// Display the image.
	screen_surface = display_image(image_surface);

	// Wait for a key to be pressed.
	wait_for_keypressed();
	
	
	Uint32 pixel;
	int w;
	int h;
	w = image_surface -> w;
	h = image_surface -> h;
	
	Uint8 r, g, b, average; 
    
	for(int i = 0; i < w; i++)
	{
		for(int j = 0; j < h; j++)
		{
			pixel = get_pixel(image_surface, i, j);
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			average = r * 0.3 + g * 0.59 + b * 0.11;
		        
			pixel = SDL_MapRGB(image_surface->format, average, average, average);
			put_pixel(image_surface,i,j,pixel);
		}
    }
 	/*update_surface(screen_surface, image_surface);
 	wait_for_keypressed();*/
 	
    //noiseReduction(image_surface);

    //image_surface = Filter(image_surface);
    //GammaCorrection(image_surface,2.2);

	//otsu(image_surface,seuil);
    //ots(image_surface);
    ot(image_surface); 

    update_surface(screen_surface, image_surface);
	wait_for_keypressed();

    int angle= RotationAuto(image_surface,0,0);

    printf("%i\n",angle);
    if (angle>35)
    {
        image_surface=SDL_RotationCentralN(image_surface,angle-1);
    }
    if (angle<0)
    {
        image_surface=SDL_RotationCentralN(image_surface,angle+1);
    }

    image_surface = kernel(image_surface);

    //GammaCorrection(image_surface,2.2);

    update_surface(screen_surface, image_surface);

    screen_surface =  display_image(image_surface);

 	wait_for_keypressed();

    SDL_SaveBMP(image_surface,"hihih.bmp");	
	SDL_FreeSurface(image_surface);

	SDL_FreeSurface(screen_surface);

	return 0;
}
