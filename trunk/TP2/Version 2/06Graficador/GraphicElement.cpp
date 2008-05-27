#include "GraphicElement.h"
#include "SDLHelper.h"
#include "Tag.h"

#ifndef GraphicElement_cpp

#define GraphicElement_cpp

void Circle::Print(SDLHelper * sdlHelper)
{
	sdlHelper->DrawCircle(this->position.x, this->position.y, this->radius, this->color, this->textura, this->GetId());
}

void GraphicElement::ValidChildElementsAndProperties(Tag * tag)
{
	ArrayList properties = tag->properties;

	if(!properties.IsEmpty())
	{
		properties.MoveFirst();
		do
		{
			TagProperty * property = (TagProperty *)properties.GetCurrent();

			char * propName = property->GetName();

			if
			(
				!(strcmp(propName, ID_ATT_NAME) == 0 ||
				strcmp(propName, FIGURE_COLOR_ATT_NAME) == 0 ||
				strcmp(propName, LINE_COLOR_ATT_NAME) == 0 ||
				strcmp(propName, TEXTURE_ATT_NAME) == 0 ||
				strcmp(propName, PATH_ATT_NAME) == 0 ||
				strcmp(propName, XPOS_ATT_NAME) == 0 ||
				strcmp(propName, YPOS_ATT_NAME) == 0 ||
				strcmp(propName, RESOLUTION_ATT_NAME) == 0 ||
				strcmp(propName, BACKCOLORGRAF_ATT_NAME ) == 0 ||
				strcmp(propName, SIDE_ATT_NAME ) == 0 ||
				strcmp(propName, WIDTH_ATT_NAME ) == 0 ||
				strcmp(propName, HEIGHT_ATT_NAME ) == 0 ||
				strcmp(propName, RADIUS_ATT_NAME) == 0)				
			)	
			{
				char * msg = StringHelper::AppendString("Invalid attribute ", propName);
				msg = StringHelper::AppendString(msg, " in  tag ");
				msg = StringHelper::AppendString(msg, tag->GetName());

				throw Exception(msg);
			}
		}
		while(properties.MoveNext());
	}
}

void Rectangle::Print(SDLHelper * sdlHelper)
{
	sdlHelper->DrawRectangle(this->position.x, this->position.y, this->width, this->height, this->color, this->textura, this->GetId());
}

void Line::Print(SDLHelper * sdlHelper)
{
	sdlHelper->DrawSegment(this->position.x, this->position.y, this->end.x, this->end.y, this->color, this->GetId());
}

#endif