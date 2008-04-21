#include "SDLHelper.h"

#ifndef GraphicElement_h

#define GraphicElement_h

#define FIGURE_COLOR_ATT_NAME "colorFigura"
#define LINE_COLOR_ATT_NAME "colorLinea"

#define POSITION_TAG_NAME "posicion"
#define BEGIN_TAG_NAME "inicio"
#define END_TAG_NAME "fin"

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
	public:
		virtual void Print(SDLHelper * sdlHelper) {};
		
		Position GetPosition() { return this->position; };
		void SetPosition(Position pos) { this->position = pos; };
		
		Color GetColor() { return this->color; };
		void SetColor(Color color) { this->color = color; };
};

class Circle : public GraphicElement
{
	private:		
		int radius;		
		char * idTextura;
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
		};

		Circle() {};

		virtual void Print(SDLHelper * sdlHelper);

		int GetRadius() { return this->radius; };
		void SetRadius(int radius) { this->radius = radius; };
};

class Rectangle : public GraphicElement
{
	protected:		
		int width;
		int height;
		char * idTextura;
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
		};

		Rectangle() {};

		virtual void Print(SDLHelper * sdlHelper);

		int GetWidth() { return this->width; };
		void SetWidth(int width) { this->width = width; };

		int GetHeight() { return this->height; };
		void SetHeight(int height) { this->height = height; };
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
		};

		Square() {};

		int GetSide() { return this->width; };
		void SetSide(int side) 
		{ 
			this->width = side; 
			this->height = side; 
		};
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