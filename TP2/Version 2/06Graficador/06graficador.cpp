#include "conio.h"
#include "stdio.h"
#include "XMLParser.h"
#include "Exception.h"
#include "SDL.h"
#include "SDLHelper.h"

#define DEBUG 1

void TestSDL();
void TestSDLHelper();
void display_bmp(char *file_name, SDL_Surface *screen);

int main(int argc, char* argv[] ) 
{

	XMLParser parser;

	try
	{
		#ifdef DEBUG
		
		Tag * rootNode = parser.ParseFile("C:\\Juan\\Facultad\\2008.1er.Taller.de.Programacion.I\\TP Nº2 Google\\06Graficador\\test.xml");

		rootNode->Print();

		#else

		if(argc > 1)
		{
			char * fileName = argv[1];

			Tag * rootNode = parser.ParseFile(fileName);

			rootNode->Print();
		}
		else
		{
			printf("\nDebe ingresar el path del archivo XML.\n");
		}

		#endif
	}
	catch(Exception ex)
	{
		printf("\nAn Exception ocurred: %s", ex.GetMessage());
	}

	//TestSDL();

	TestSDLHelper();

	return 0;
}

void TestSDLHelper()
{
	SDLHelper sdlHelper;
	sdlHelper.Initialize(800, 600, 32);

	Color color;
	color.R = 255;
	color.G = 255;
	color.B = 255;

	sdlHelper.DrawCircle(10, 10, 10, color, 500);
	sdlHelper.Refresh();

	getch();

	sdlHelper.Quit();
}

void TestSDL()
{
	SDL_Surface *screen;

    printf("Initializing SDL.\n");
    
    /* Initialize defaults, Video and Audio */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) { 
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }

    printf("SDL initialized.\n");

	screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set 640x480x32 video mode: %s\n",
                        SDL_GetError());
        exit(1);
    }

    display_bmp("C:\\untitled.bmp", screen);

	printf("Quiting SDL.\n");
    
    /* Shutdown all subsystems */
    SDL_Quit();
    
    printf("Quiting....\n");

    exit(0);
}

void display_bmp(char *file_name, SDL_Surface *screen)
{
    SDL_Surface *image;

    /* Load the BMP file into a surface */
    image = SDL_LoadBMP(file_name);
    if (image == NULL) {
        fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
        return;
    }

    /*
     * Palettized screen modes will have a default palette (a standard
     * 8*8*4 colour cube), but if the image is palettized as well we can
     * use that palette for a nicer colour matching
     */
    if (image->format->palette && screen->format->palette) {
    SDL_SetColors(screen, image->format->palette->colors, 0,
                  image->format->palette->ncolors);
    }

    /* Blit onto the screen surface */
    if(SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);

	getch();

    /* Free the allocated BMP surface */
    SDL_FreeSurface(image);
}