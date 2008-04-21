#include "SDLHelper.h"
#include "iostream.h"
#include "Math.h"

#ifndef SDLHelper_cpp

#define SDLHelper_cpp


SDLHelper::SDLHelper()
{
	this->screen = NULL;
}

void SDLHelper::Initialize(int widthRes, int heightRes, int colorDepth)
{
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1))
	{
		throw Exception(SDL_GetError());
		exit(1);
    }	

	screen = SDL_SetVideoMode(widthRes, heightRes, colorDepth, SDL_SWSURFACE);

    if ( screen == NULL ) 
	{
		throw Exception(SDL_GetError());
		exit(1);
    }	
}

void SDLHelper::Quit()
{
    SDL_Quit();
}

// DrawPixel 
//   Tomada del tutorial de sdl
//   http://www.libsdl.org/intro.en/usingvideo.html

void SDLHelper::DrawPixel(SDL_Surface * screen, int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
	if ( SDL_MUSTLOCK(screen) ) 
	{
        if ( SDL_LockSurface(screen) < 0 ) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return;
        }
    }

	if ( x < 0 || x >= screen->clip_rect.w || y < 0 || y >= screen->clip_rect.h )
		return;

    Uint32 color = SDL_MapRGB(screen->format, R, G, B);

    switch (screen->format->BytesPerPixel) {
        case 1: { /* Assuming 8-bpp */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *bufp = color;
        }
        break;

        case 2: { /* Probably 15-bpp or 16-bpp */
            Uint16 *bufp;

            bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
            *bufp = color;
        }
        break;

        case 3: { /* Slow 24-bpp mode, usually not used */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *(bufp+screen->format->Rshift/8) = R;
            *(bufp+screen->format->Gshift/8) = G;
            *(bufp+screen->format->Bshift/8) = B;
        }
        break;

        case 4: { /* Probably 32-bpp */
            Uint32 *bufp;

            bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
            *bufp = color;
        }
        break;
    }

    if ( SDL_MUSTLOCK(screen) ) 
	{
        SDL_UnlockSurface(screen);
    }
}

void SDLHelper::GetPixel(SDL_Surface * screen, int x, int y, Uint8* R, Uint8* G, Uint8* B)
{
	x = x % screen->clip_rect.w;
	y = y % screen->clip_rect.h;

    switch (screen->format->BytesPerPixel) {
        case 1: { /* Assuming 8-bpp */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
			SDL_GetRGB ( *bufp , screen->format , R , G , B );
        }
        break;

        case 2: { /* Probably 15-bpp or 16-bpp */
            Uint16 *bufp;

            bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
			SDL_GetRGB ( *bufp , screen->format , R , G , B );
        }
        break;

        case 3: { /* Slow 24-bpp mode, usually not used */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
			SDL_GetRGB ( *bufp , screen->format , R , G , B );
        }
        break;

        case 4: { /* Probably 32-bpp */
            Uint32 *bufp;

            bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
			SDL_GetRGB ( *bufp , screen->format , R , G , B );
        }
        break;
    }
}

void SDLHelper::DrawSquare ( int x , int y , int l , Color color , SDL_Surface* texture , int nodeId )
{
	if ( x < 0 || y < 0 || l <= 0 )
	{
		cout << "Warning!!! Invalid square coordinates (" << x << "," << y 
			<< "," << l << ") in node " << nodeId << endl;
		return;
	}

	for ( int posY = y ; posY < (y + l) ; posY++ )
		for ( int posX = x ; posX < (x + l) ; posX++ )
		{
			if ( texture != NULL )
				GetPixel ( texture , posX - x , posY - y , &color.R , &color.G , &color.B );

			DrawPixel ( screen , posX , posY , color.R , color.G , color.B );
		}
}

void SDLHelper::DrawCircle ( int x , int y , int r , Color color , int nodeId )
{
	if ( x < 0 || y < 0 || r <= 0 )
	{
		cout << "Warning!!! Invalid circle coordinates (" << x << "," << y 
			<< "," << r << ") in node " << nodeId << endl;
		return;
	}

	for ( int posY = y - r ; posY <= (y + r) ; posY++ )
	{
		int dY = abs(y - posY);
		int dX = (int) sqrt ( (double) (r * r - dY * dY) );

		for ( int posX = x - dX ; posX <= (x + dX) ; posX++ )
			DrawPixel ( screen , posX , posY , color.R , color.G , color.B );
	}
}

void SDLHelper::DrawRectangle ( int x , int y , int b , int h , Color color , int nodeId )
{
	if ( x < 0 || y < 0 || b <= 0 || h <= 0 )
	{
		cout << "Warning!!! Invalid rectangle coordinates (" << x << "," << y 
			<< "," << b << "," << h << ") in node " << nodeId << endl;
		return;
	}

	for ( int posY = y ; posY < (y + h) ; posY++ )
		for ( int posX = x ; posX < (x + b) ; posX++ )
			DrawPixel ( screen , posX , posY , color.R , color.G , color.B );
}

#define USE_ANTIALIASING	

#ifndef USE_ANTIALIASING
#define CALCULATE_ANTIALIAS(c1,c2,ref,offset) int c1 = 0, c2 = ref;
#else
#define CALCULATE_ANTIALIAS(c1,c2,ref,offset) \
	int c1 = (int) ((double) ref * (offset - (double) (int) offset)); \
	int c2 = ref - c1; \
	if ( c1 > c2 )	corr = sqrt((double) (c1 + c2) / (double) c1); \
	else			corr = sqrt((double) (c1 + c2) / (double) c2); \
	c1 = (int) (corr * (double) c1); \
	c2 = (int) (corr * (double) c2);
#endif

void SDLHelper::DrawSegment ( int x1 , int y1 , int x2 , int y2 , Color color , int nodeId )
{
	if ( x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0 )
	{
		cout << "Warning!!! Invalid segment coordinates (" << x1 << "," << y1 
			<< "-" << x2 << "," << y2 << ") in node " << nodeId << endl;
		return;
	}

	if ( x2 < x1 )
	{
		int tmpx, tmpy;
		tmpx = x1;
		x1 = x2;
		x2 = tmpx;
		tmpy = y1;
		y1 = y2;
		y2 = tmpy;
	}

	#ifdef USE_ANTIALIASING
	double corr = 0;
	#endif

	if ( (x2 - x1) > (y2 - y1) )
	{
		double y = y1;
		for ( int posX = x1 ; posX <= x2 ; posX++ )
		{
			CALCULATE_ANTIALIAS(r1,r2,color.R,y);
			CALCULATE_ANTIALIAS(g1,g2,color.G,y);
			CALCULATE_ANTIALIAS(b1,b2,color.B,y);

			DrawPixel ( screen , posX , (int) y , r2 , g2 , b2 );
			DrawPixel ( screen , posX , (int) y + 1 , r1 , g1 , b1 );
			y += (double) (y2 - y1) / (double) (x2 - x1);
		}
	}
	else
	{
		double x = x1;
		for ( int posY = y1 ; posY <= y2 ; posY++ )
		{
			CALCULATE_ANTIALIAS(r1,r2,color.R,x);
			CALCULATE_ANTIALIAS(g1,g2,color.G,x);
			CALCULATE_ANTIALIAS(b1,b2,color.B,x);

			DrawPixel ( screen , (int) x , posY , r2 , g2 , b2 );
			DrawPixel ( screen , (int) x + 1 , posY , r1 , g1 , b1 );
			x += (double) (x2 - x1) / (double) (y2 - y1);
		}
	}
}


void SDLHelper::Refresh()
{
	SDL_UpdateRect(this->screen, 0, 0, this->screen->w, this->screen->h);
}

void SDLHelper::WaitForKey()
{
	SDL_Event event;

	bool keyPress = false;

	while(!keyPress)
	{
		SDL_PollEvent( &event );
        switch( event.type ){
            /* Keyboard event */
            /* Pass the event data onto PrintKeyInfo() */
            case SDL_KEYDOWN:
            case SDL_KEYUP:
				keyPress = true;
                break;
        }
    }
}

#endif