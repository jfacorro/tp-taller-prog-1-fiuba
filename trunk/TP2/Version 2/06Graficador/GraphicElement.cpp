#include "GraphicElement.h"
#include "SDLHelper.h"

#ifndef GraphicElement_cpp

#define GraphicElement_cpp

void Circle::Print(SDLHelper * sdlHelper)
{
	sdlHelper->DrawCircle(this->position.x, this->position.y, this->radius, this->color, 0);
}

void Rectangle::Print(SDLHelper * sdlHelper)
{
	sdlHelper->DrawRectangle(this->position.x, this->position.y, this->width, this->height, this->color, 0);
}

void Line::Print(SDLHelper * sdlHelper)
{
	sdlHelper->DrawSegment(this->position.x, this->position.y, this->end.x, this->end.y, this->color, 0);
}

#endif