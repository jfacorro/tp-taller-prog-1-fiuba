// xml.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "XmlDictionaryParser.h"
#include <iostream>
#include <set>
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

void GetPixel(SDL_Surface *screen, int x, int y, Uint8* R, Uint8* G, Uint8* B)
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

typedef struct _Color
{
	Uint8 R;
	Uint8 G;
	Uint8 B;
}
	Color;

void DrawSquare ( SDL_Surface* screen , int x , int y , int l , Color color , SDL_Surface* texture , int nodeId )
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

void DrawCircle ( SDL_Surface* screen , int x , int y , int r , Color color , int nodeId )
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

void DrawRectangle ( SDL_Surface* screen , int x , int y , int b , int h , Color color , int nodeId )
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

void DrawSegment ( SDL_Surface* screen , int x1 , int y1 , int x2 , int y2 , Color color , int nodeId )
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

map<string,SDL_Surface*> textures;

SDL_Surface* GetTexture ( XmlDictionaryNode* p )
{
	SDL_Surface* retval = NULL;

	map<string,string>::iterator iter = p->nodeAttrs.find("textura");
	if ( iter != p->nodeAttrs.end() )
	{
		map<string,SDL_Surface*>::iterator texture = textures.find(iter->second);
		if ( texture == textures.end() )
			cout << "Warning!!! Texture not found in node " << p->nodeId << endl;
		else
			retval = texture->second;
	}

	return retval;
}

#define PRINT_COLOR_SYNTAX_ERROR() 	cout << "Warning!!! Invalid color syntax in node " << p->nodeId << endl

Color GetColorAttribute ( XmlDictionaryNode* p , string attrName )
{
	Color retval;
	retval.R = 255;
	retval.G = 255;
	retval.B = 255;

	map<string,string>::iterator iter = p->nodeAttrs.find(attrName);
	if ( iter != p->nodeAttrs.end () )
	{
		if ( iter->second.size() != 9 )
			PRINT_COLOR_SYNTAX_ERROR();
		else
		{
			int tmpR, tmpG, tmpB;
			sscanf ( iter->second.substr(0,3).c_str() , "%d" , &tmpR );
			sscanf ( iter->second.substr(3,3).c_str() , "%d" , &tmpG );
			sscanf ( iter->second.substr(6,3).c_str() , "%d" , &tmpB );

			if ( tmpR < 0 || tmpR > 255 || 
				 tmpG < 0 || tmpG > 255 ||
				 tmpB < 0 || tmpB > 255 )
			{
				PRINT_COLOR_SYNTAX_ERROR();
			}
			else
			{
				retval.R = tmpR;
				retval.G = tmpG;
				retval.B = tmpB;
			}
		}
	}

	return retval;
}

void LoadTexture ( XmlDictionaryNode* p )
{
	map<string,string>::iterator id = p->nodeAttrs.find("id");
	map<string,string>::iterator path = p->nodeAttrs.find("path");

	if ( id == p->nodeAttrs.end() || path == p->nodeAttrs.end() )
		cout << "Warning!!! Attributes 'id' and 'path' are mandatory in node "
			<< p->nodeId << endl;
	else
	{
		if ( textures.find(id->second) != textures.end() )
			cout << "Warning!!! Texture '" << id->second << "' already loaded in node " 
				<< p->nodeId << endl;
		else
		{
			SDL_Surface* bmp = SDL_LoadBMP ( path->second.c_str() );
			if ( bmp == NULL )
				cout << "Warning!!! Invalid texture file '" << path->second 
					<< "' in node " << p->nodeId << endl;
			else
				textures.insert(make_pair(id->second,bmp));
		}
	}
}

void ParseDictionary ( SDL_Surface* screen , XmlDictionaryNode* p )
{
	XmlDictionaryNode* current = p->children;
	set<string> ids;
	Point* pos = NULL, *pos2 = NULL;
	int p1,p2;

	while ( current != NULL )
	{
		if ( current->nodeName == "cuadrado" )
		{
			pos = GetPosition ( current , "posicion" );
			if ( pos != NULL && GetAttribute(current,"lado",&p1) )
				DrawSquare ( screen , pos->X , pos->Y , p1 , 
					GetColorAttribute(current,"colorFigura") , 
					GetTexture(current) , current->nodeId );
		}
		else if ( current->nodeName == "circulo" )
		{
			pos = GetPosition ( current , "posicion"  );
			if ( pos != NULL && GetAttribute(current,"radio",&p1) )
				DrawCircle ( screen , pos->X , pos->Y , p1 , 
					GetColorAttribute(current,"colorFigura") , current->nodeId );
		}
		else if ( current->nodeName == "rectangulo" )
		{
			pos = GetPosition ( current , "posicion" );
			if ( pos != NULL && GetAttribute(current,"base",&p1) &&
				 GetAttribute(current,"altura",&p2) )
			{
				DrawRectangle ( screen , pos->X , pos->Y , p1 , p2 , 
					GetColorAttribute(current,"colorFigura") , current->nodeId );
			}
		}
		else if ( current->nodeName == "segmento" )
		{
			pos  = GetPosition ( current , "inicio" );
			pos2 = GetPosition ( current , "fin" );

			if ( pos != NULL && pos2 != NULL )
				DrawSegment ( screen , pos->X , pos->Y , pos2->X , pos2->Y , 
					GetColorAttribute(current,"colorLinea") , current->nodeId );

			if ( pos2 != NULL )
			{
				delete pos2;
				pos2 = NULL;
			}
		}
		else if ( current->nodeName == "textura" )
			LoadTexture ( current );
		else
			cout << "Warning!!! Unrecognized shape '" << current->nodeName 
				<< "' in node " << current->nodeId << endl;

		map<string,string>::iterator iter = current->nodeAttrs.find("id");
		if ( iter == current->nodeAttrs.end() )
			cout << "Warning!!! Attribute 'id' is  mandatory in node " 
				<< current->nodeId << endl;
		else
		{
			if ( ids.find(iter->second) != ids.end() )
				cout << "Warning!!! Attribute 'id' is not unique in node "
					<< current->nodeId << endl;
			else
				ids.insert(iter->second);
		}

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
			(string) "<textura id='stone' path='C:\\Archivos de programa\\Microsoft Office\\Office10\\Bitmaps\\Styles\\stone.bmp'/>" +
			(string) "<textura id='cafe' path='C:\\windows\\grano de café.bmp'/>" +
			(string) "<cuadrado id='cuadrado1' textura='stone' colorFigura='255000000' lado='10'><posicion x='0' y='0' /></cuadrado>" +
			(string) "<cuadrado id='cuadrado2' colorFigura='000255000' lado='200' textura='cafe'><posicion x='100' y='100' /></cuadrado>" +
			(string) "<rectangulo id='r1' colorFigura='000000255' base='10' altura='20'><posicion x='0' y='100'/></rectangulo>" +
			(string) "<segmento id='s1' colorLinea='100200100'><inicio x='0' y='0' /><fin x='639' y='20' /></segmento>" +
			(string) "<segmento id='s2' colorLinea='200100200'><inicio x='0' y='0' /><fin x='20' y='479' /></segmento>" +
			(string) "<circulo id='c1'colorFigura='255255255' radio='30'><posicion x='200' y='200' /></circulo>" +
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

