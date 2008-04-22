#include "SDL.h"
#include "Common.h"
#include "Exception.h"

#ifndef SDLHelper_h

#define SDLHelper_h

typedef struct _Color
{
	Uint8 R;
	Uint8 G;
	Uint8 B;
}
Color;

class SDLHelper
{
	private:
		SDL_Surface * screen;

		void DrawPixel(SDL_Surface * screen, int x, int y, Uint8 R, Uint8 G, Uint8 B);
		void GetPixel(SDL_Surface * screen, int x, int y, Uint8* R, Uint8* G, Uint8* B);
	public:
		SDLHelper();
		~SDLHelper() {};

		void Initialize(int widthRes, int heightRes, int colorDepth);
		void Quit();

		void DrawSquare ( int x , int y , int l , Color color , SDL_Surface* texture , char * nodeId );
		void DrawCircle ( int x , int y , int r , Color color , SDL_Surface* texture , char * nodeId );
		void DrawRectangle ( int x , int y , int b , int h , Color color , SDL_Surface* texture , char * nodeId );
		void DrawSegment ( int x1 , int y1 , int x2 , int y2 , Color color , char * nodeId );

		static SDL_Surface * LoadBitmap(char * bitmap);

		void Refresh();

		void WaitForKey();
};

#endif