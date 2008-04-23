#include "SDL.h"
#include "Common.h"
#include "Exception.h"
#include "StringHelper.h"

#ifndef SDLHelper_h

#define SDLHelper_h

typedef struct _Color
{
	Uint8 R;
	Uint8 G;
	Uint8 B;
}
Color;

typedef struct _Resolution
{
	int w;
	int h;
}
Resolution;

class Texture
{
	private:
		char * id;
		SDL_Surface * bitmap;
	public:
		char * GetId() { return this->id; };
		void SetId(char * id) { this->id  = StringHelper::Substring(id, 0, strlen(id)); };

		SDL_Surface * GetBitmap() { return this->bitmap; };
		void SetBitmap(SDL_Surface * bitmap) { this->bitmap  = bitmap; };
};

class Configuration
{
	private:
		Resolution resolucion;
		Color colorFondoGraf;
		Texture * textura;
		Color colorLinea;
		Color colorFondo;
	public:
		Configuration();

		/// Accessors
		Resolution GetResolucion() { return this->resolucion; };
		void SetResolucion(Resolution resolucion) { this->resolucion = resolucion; };

		Color GetColorFondoGraf() { return this->colorFondoGraf; };
		void SetColorFondoGraf(Color colorFondoGraf) { this->colorFondoGraf = colorFondoGraf; };

		Texture * GetTextura() { return this->textura; };
		void SetTextura(Texture * textura) { this->textura = textura; };

		Color GetColorLinea() { return this->colorLinea; };
		void SetColorLinea(Color colorLinea) { this->colorLinea = colorLinea; };

		Color GetColorFondo() { return this->colorFondo; };
		void SetColorFondo(Color colorFondo) { this->colorFondo = colorFondo; };
};


class SDLHelper
{
	private:
		SDL_Surface * screen;

		void DrawPixel(SDL_Surface * screen, int x, int y, Uint8 R, Uint8 G, Uint8 B);
		void GetPixel(SDL_Surface * screen, int x, int y, Uint8* R, Uint8* G, Uint8* B);
	public:
		SDLHelper();
		~SDLHelper() {};

		void Initialize(Configuration config);
		void Quit();

		void DrawSquare ( int x , int y , int l , Color color , SDL_Surface* texture , char * nodeId );
		void DrawCircle ( int x , int y , int r , Color color , SDL_Surface* texture , char * nodeId );
		void DrawRectangle ( int x , int y , int b , int h , Color color , SDL_Surface* texture , char * nodeId );
		void DrawSegment ( int x1 , int y1 , int x2 , int y2 , Color color , char * nodeId );

		void Refresh();

		void WaitForKey();

		static SDL_Surface * LoadBitmap(char * bitmap);

		static Color GetDefaultFrontColor()
		{
			Color color;
			color.R = 0;
			color.G = 0;
			color.B = 0;
			return color;
		};

		static Color GetDefaultBackColor()
		{
			Color color;
			color.R = 255;
			color.G = 255;
			color.B = 255;
			return color;
		};

		static Resolution ResolutionByWidth(int width);
};

#endif