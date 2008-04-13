// xml.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "XmlDictionaryParser.h"
#include <iostream>
#include "conio.h"
#include "sdl.h"
#include "math.h"


void PrintDictionary ( XmlDictionaryNode* p , string indent )
{
	cout << indent << "(" << p->nodeId << ") " << p->nodeName << " {";

	map<string,string>::iterator iter;
	for ( iter = p->nodeAttrs.begin() ; iter != p->nodeAttrs.end() ; ++iter )
		cout << "\"" << iter->first << "\":\"" << iter->second << "\",";
	
	cout << "}" << endl;

	XmlDictionaryNode* c = p->children;
	while ( c != NULL )
	{
		PrintDictionary ( c , indent + "  " );
		c = c->next;
	}
}

bool GetAttribute ( XmlDictionaryNode* p , string attrName , int* val )
{
	map<string,string>::iterator iter = p->nodeAttrs.find(attrName);
	if ( iter == p->nodeAttrs.end() )
	{
		cout << "Warning!!! Missing attribute '" << attrName << "' in node " 
			<< p->nodeId << endl;
		return false;
	}

	// TODO: Verificar validez de val
	sscanf ( iter->second.c_str() , "%d" , val );
	return true;
}

typedef struct _Point
{
	int X;
	int Y;
}
	Point;

Point* GetPosition ( XmlDictionaryNode* p , string tagName )
{
	int x, y;
	XmlDictionaryNode* tmp = p->children;
	while ( tmp != NULL )
	{
		if ( tmp->nodeName == tagName )
		{
			map<string,string>::iterator iter = tmp->nodeAttrs.find("x");
			if ( iter == tmp->nodeAttrs.end() )
			{
				cout << "Warning!!! Missing position parameter 'x' in node " 
					<< tmp->nodeId << " (parent=" << p->nodeId << ")" << endl;
				return NULL;
			}
			
			// TODO: Verificar validez de x
			sscanf ( iter->second.c_str() , "%d" , &x );

			iter = tmp->nodeAttrs.find("y");
			if ( iter == tmp->nodeAttrs.end() )
			{
				cout << "Warning!!! Missing position parameter 'y' in node " 
					<< tmp->nodeId << " (parent=" << p->nodeId << ")" << endl;
				return NULL;
			}
	
			// TODO: Verificar validez de x
			sscanf ( iter->second.c_str() , "%d" , &y );

			Point* retval = new Point();
			retval->X = x;
			retval->Y = y;
			return retval;
		}

		tmp = tmp->next;
	}

	cout << "Warning!!! Missing position tag under node " << p->nodeId << endl;
	return NULL;
}

// DrawPixel 
//   Tomada del tutorial de sdl
//   http://www.libsdl.org/intro.en/usingvideo.html

void DrawPixel(SDL_Surface *screen, int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
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
}

void DrawSquare ( SDL_Surface* screen , int x , int y , int l , int nodeId )
{
	if ( x < 0 || y < 0 || l <= 0 )
	{
		cout << "Warning!!! Invalid square coordinates (" << x << "," << y 
			<< "," << l << ") in node " << nodeId << endl;
		return;
	}

	for ( int posY = y ; posY < (y + l) ; posY++ )
		for ( int posX = x ; posX < (x + l) ; posX++ )
			DrawPixel ( screen , posX , posY , 255 , 0 , 0 );
}

void DrawCircle ( SDL_Surface* screen , int x , int y , int r , int nodeId )
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
			DrawPixel ( screen , posX , posY , 255 , 0 , 0 );
	}
}

void DrawRectangle ( SDL_Surface* screen , int x , int y , int b , int h , int nodeId )
{
	if ( x < 0 || y < 0 || b <= 0 || h <= 0 )
	{
		cout << "Warning!!! Invalid rectangle coordinates (" << x << "," << y 
			<< "," << b << "," << h << ") in node " << nodeId << endl;
		return;
	}

	for ( int posY = y ; posY < (y + h) ; posY++ )
		for ( int posX = x ; posX < (x + b) ; posX++ )
			DrawPixel ( screen , posX , posY , 255 , 0 , 0 );
}

#define USE_ANTIALIASING	

void DrawSegment ( SDL_Surface* screen , int x1 , int y1 , int x2 , int y2 , int nodeId )
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

	if ( (x2 - x1) > (y2 - y1) )
	{
		double y = y1;
		for ( int posX = x1 ; posX <= x2 ; posX++ )
		{
			#ifndef USE_ANTIALIASING
			int r1 = 0;
			int r2 = 255;
			#else
			// Un poco de antialiasing...
			int r1 = (int) ((double) 255 * (y - (double) (int) y));
			int r2 = 255 - r1;

			double corr = 0;
			if ( r1 > r2 )
				corr = sqrt((double) (r1 + r2) / (double) r1);
			else
				corr = sqrt((double) (r1 + r2) / (double) r2);

			r1 = (int) (corr * (double) r1);
			r2 = (int) (corr * (double) r2);
			#endif

			DrawPixel ( screen , posX , (int) y , r2 , 0 , 0 );
			DrawPixel ( screen , posX , (int) y + 1 , r1 , 0 , 0 );
			y += (double) (y2 - y1) / (double) (x2 - x1);
		}
	}
	else
	{
		double x = x1;
		for ( int posY = y1 ; posY <= y2 ; posY++ )
		{
			#ifndef USE_ANTIALIASING
			int r1 = 0;
			int r2 = 255;
			#else
			int r1 = (int) ((double) 255 * (x - (double) (int) x));
			int r2 = 255 - r1;

			double corr = 0;
			if ( r1 > r2 )
				corr = sqrt((double) (r1 + r2) / (double) r1);
			else
				corr = sqrt((double) (r1 + r2) / (double) r2);

			r1 = (int) (corr * (double) r1);
			r2 = (int) (corr * (double) r2);
			#endif

			DrawPixel ( screen , (int) x , posY , r2 , 0 , 0 );
			DrawPixel ( screen , (int) x + 1 , posY , r1 , 0 , 0 );
			x += (double) (x2 - x1) / (double) (y2 - y1);
		}
	}
}

void ParseDictionary ( SDL_Surface* screen , XmlDictionaryNode* p )
{
	XmlDictionaryNode* current = p->children;
	list<string> ids;
	Point* pos = NULL, *pos2 = NULL;
	int p1,p2;

	while ( current != NULL )
	{
		if ( current->nodeName == "cuadrado" )
		{
			pos = GetPosition ( current , "posicion" );
			if ( pos != NULL && GetAttribute(current,"lado",&p1) )
				DrawSquare ( screen , pos->X , pos->Y , p1 , current->nodeId );
		}
		else if ( current->nodeName == "circulo" )
		{
			pos = GetPosition ( current , "posicion"  );
			if ( pos != NULL && GetAttribute(current,"radio",&p1) )
				DrawCircle ( screen , pos->X , pos->Y , p1 , current->nodeId );
		}
		else if ( current->nodeName == "rectangulo" )
		{
			pos = GetPosition ( current , "posicion" );
			if ( pos != NULL && GetAttribute(current,"base",&p1) &&
				 GetAttribute(current,"altura",&p2) )
			{
				DrawRectangle ( screen , pos->X , pos->Y , p1 , p2 , current->nodeId );
			}
		}
		else if ( current->nodeName == "segmento" )
		{
			pos  = GetPosition ( current , "inicio" );
			pos2 = GetPosition ( current , "fin" );

			if ( pos != NULL && pos2 != NULL )
				DrawSegment ( screen , pos->X , pos->Y , pos2->X , pos2->Y , current->nodeId );

			if ( pos2 != NULL )
			{
				delete pos2;
				pos2 = NULL;
			}
		}
		else
			cout << "Warning!!! Unrecognized shape '" << current->nodeName << "'" << endl;

		current = current->next;
		if ( pos != NULL )
		{
			delete pos;
			pos = NULL;
		}
	}
}

void PrintShapes ( XmlDictionaryNode* p )
{
	SDL_Surface* screen;
	screen = SDL_SetVideoMode ( 640 , 480 , 16, SDL_SWSURFACE );
	if ( screen == NULL )
	{
		printf ( "Unable to set 640x480 video: %s\n", SDL_GetError() );
		return;
	}

    if ( SDL_MUSTLOCK(screen) ) 
        if ( SDL_LockSurface(screen) < 0 ) 
		{
			printf ( "Unable lock surface: %s\n", SDL_GetError() );
            return;
        }

	ParseDictionary ( screen , p );	

    if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }
    SDL_UpdateRect(screen, 0, 0, 640, 480);
}

int _tmain(int argc, _TCHAR* argv[])
{
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
		cout << "Unable to initialize SDL: " << SDL_GetError() << endl;
	else
	{
		string xml = (string) "<root>" +
			(string) "<cuadrado id='cuadrado1' lado='10'><posicion x='0' y='0' /></cuadrado>" +
			(string) "<cuadrado id='cuadrado1' lado='50'><posicion x='100' y='100' /></cuadrado>" +
			(string) "<rectangulo id='r1' base='10' altura='20'><posicion x='0' y='100'/></rectangulo>" +
			(string) "<segmento id='s1'><inicio x='0' y='0' /><fin x='639' y='20' /></segmento>" +
			(string) "<segmento id='s1'><inicio x='0' y='0' /><fin x='20' y='479' /></segmento>" +
			(string) "<circulo id='c1' radio='30'><posicion x='200' y='200' /></circulo>" +
			(string) "</root>";
		XmlDictionaryParser parser(xml);
		parser.Parse();

		XmlDictionaryNode* d = parser.GetRoot();
		if ( d != NULL )
		{
			cout << endl << "Dumping dictionary..." << endl;
			PrintDictionary ( d , "" );
			cout << endl << "Printing shapes... ";
			PrintShapes ( d );
			cout << "Ready." << endl;
		}

		cout << "Press any key to continue...";
		getch();
		//int k;
		//cin >> k;
		SDL_Quit();
	}

	return 0;
}

