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
		char * filePath;
		SDL_Surface * bitmap;

	public:
		char * GetId() { return this->id; };
		void SetId(char * id) { this->id  = StringHelper::Substring(id, 0, strlen(id)); };

		char * GetFilePath() { return this->filePath; };
		void SetFilePath(char * filePath) { this->filePath  = StringHelper::Substring(filePath, 0, strlen(filePath)); };

		SDL_Surface * GetBitmap() { return this->bitmap; };
		void SetBitmap(SDL_Surface * bitmap) { this->bitmap  = bitmap; };
};

class Configuration
{
	private:
		Resolution resolucion;
		Color colorFondoGraf;
		SDL_Surface * textura;
		Color colorLinea;
		Color colorFondo;

		bool isDefaultConfig;
	public:
		Configuration();

		/// Accessors
		Resolution GetResolucion() { return this->resolucion; };
		void SetResolucion(Resolution resolucion) { this->isDefaultConfig = false; this->resolucion = resolucion; };

		Color GetColorFondoGraf() { return this->colorFondoGraf; };
		void SetColorFondoGraf(Color colorFondoGraf) { this->isDefaultConfig = false; this->colorFondoGraf = colorFondoGraf; };

		SDL_Surface * GetTextura() { return this->textura; };
		void SetTextura(SDL_Surface * textura);

		Color GetColorLinea() { return this->colorLinea; };
		void SetColorLinea(Color colorLinea) { this->isDefaultConfig = false; this->colorLinea = colorLinea; };

		Color GetColorFondo() { return this->colorFondo; };
		void SetColorFondo(Color colorFondo) { this->isDefaultConfig = false; this->colorFondo = colorFondo; };

		bool GetIsDefaultConfig() { return this->isDefaultConfig; };
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

		void Initialize();
		void InitializeVideo(Configuration config);
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
		static SDL_Surface * SDLResizeBitmap(SDL_Surface * image, int new_w, int new_h, int filter = 7);
};

#endif