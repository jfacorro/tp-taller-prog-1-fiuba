#include "SDL.h"
#include "SDLHelper.h"
#include "StringHelper.h"
#include "Tag.h"

#ifndef GraphicElement_h

#define GraphicElement_h

#define ID_ATT_NAME "id"

#define FIGURE_COLOR_ATT_NAME "colorFigura"
#define LINE_COLOR_ATT_NAME "colorLinea"

#define TEXTURE_ATT_NAME "textura"
#define PATH_ATT_NAME "path"

#define POSITION_TAG_NAME "posicion"
#define XPOS_ATT_NAME "x"
#define YPOS_ATT_NAME "y"

#define RADIUS_ATT_NAME "radio"

#define SIDE_ATT_NAME "lado"

#define WIDTH_ATT_NAME "base"
#define HEIGHT_ATT_NAME "altura"

#define BEGIN_TAG_NAME "inicio"
#define END_TAG_NAME "fin"

#define LINE_TAG_NAME "segmento"
#define RECTANGLE_TAG_NAME "rectangulo"
#define CIRCLE_TAG_NAME "circulo"
#define SQUARE_TAG_NAME "cuadrado"

#define GENERAL_TAG_NAME "general"

#define RESOLUTION_ATT_NAME "resolucion"
#define BACKCOLORGRAF_ATT_NAME "colorFondoGraf"

typedef struct Position
{
	int x;
	int y;
}
Position;

class GraphicElement
{
	protected:
		Position position;	
		Color color;
		char * id;
	public:
		virtual void Print(SDLHelper * sdlHelper) {};
		static void ValidChildElementsAndProperties(Tag * tag);
		
		Position GetPosition() { return this->position; };
		void SetPosition(Position pos) { this->position = pos; };
		
		Color GetColor() { return this->color; };
		void SetColor(Color color) { this->color = color; };

		char * GetId() { return this->id; };
		void SetId(char * id) { this->id  = StringHelper::Substring(id, 0, strlen(id)); };
};

class Circle : public GraphicElement
{
	private:		
		int radius;		
		char * idTextura;
		SDL_Surface * textura;
	public:
		Circle(int radius, Position pos, Color color)
		{
			this->color = color;
			Circle(radius, pos);
		};

		Circle(int radius, Position pos)
		{
			this->radius = radius;
			this->position = pos;
			Circle();
		};

		Circle() 
		{
			this->textura = NULL;
		};

		virtual void Print(SDLHelper * sdlHelper);

		//virtual bool ValidChildElementsAndProperties(Tag * tag);

		int GetRadius() { return this->radius; };
		void SetRadius(int radius) { this->radius = radius; };

		SDL_Surface * GetTextura() { return this->textura; };
		void SetTextura(SDL_Surface * textura) 
		{
			this->textura = SDLHelper::SDLResizeBitmap(textura, this->radius * 2, this->radius * 2, 7);
		};
};

class Rectangle : public GraphicElement
{
	protected:		
		int width;
		int height;
		char * idTextura;
		SDL_Surface * textura;
	public:
		Rectangle(int width, int height, Position pos, Color color)
		{
			this->color = color;
			Rectangle(width, height, pos);
		};

		Rectangle(int width, int height, Position pos)
		{
			this->width = width;
			this->height = height;
			this->position = pos;
			Rectangle();
		};

		Rectangle() 
		{
			this->textura = NULL;
		};

		virtual void Print(SDLHelper * sdlHelper);

		//virtual bool ValidChildElementsAndProperties(Tag * tag);

		int GetWidth() { return this->width; };
		void SetWidth(int width) { this->width = width; };

		int GetHeight() { return this->height; };
		void SetHeight(int height) { this->height = height; };

		SDL_Surface * GetTextura() { return this->textura; };
		void SetTextura(SDL_Surface * texturaParam) 
		{ 			
			this->textura = SDLHelper::SDLResizeBitmap(texturaParam, this->width, this->height, 7);
		};
};

class Square : public Rectangle
{
	public:
		Square(int side, Position pos, Color color)
		{
			this->color = color;
			Square(side, pos);
		};

		Square(int side, Position pos)
		{
			this->width = side;
			this->height = side;
			this->position = pos;
			Square();
		};

		Square() 
		{
			this->textura = NULL;
		};

		int GetSide() { return this->width; };
		void SetSide(int side) 
		{ 
			this->width = side; 
			this->height = side; 
		};

		//virtual bool ValidChildElementsAndProperties(Tag * tag);
};

class Line : public GraphicElement
{
	private:
		Position end;
	public:
		Line(Position begin, Position end, Color color) 
		{
			this->color = color;
			Line(begin, end);
		};

		Line(Position begin, Position end) 
		{
			this->position = begin;
			this->end = end;
		};

		Line() {};

		virtual void Print(SDLHelper * sdlHelper);

		Position GetBegin() { return this->position; };
		void SetBegin(Position position) { this->position = position; };

		Position GetEnd() { return this->end; };
		void SetEnd(Position end) { this->end = end; };
};

#endif

