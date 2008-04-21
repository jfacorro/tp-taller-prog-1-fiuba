#include "ModelValidator.h"
#include "GraphicElement.h"

#ifndef ModelValidator_cpp

#define ModelValidator_cpp

void ModelValidator::ParseAndValidate(Tag * rootNode)
{
	ArrayList children = rootNode->childTags;

	if(!children.IsEmpty())
	{
		children.MoveFirst();

		do
		{
			Tag * child = (Tag *) children.GetCurrent();

			GraphicElement * graphElem = GetGraphicElement(child);

			if(graphElem != NULL)
			{			
				this->graphicElements.Add(graphElem);
			}
			
		}while(children.MoveNext());
	}
}

GraphicElement * ModelValidator::GetGraphicElement(Tag * tag)
{
	char * name = tag->GetName();

	GraphicElement * graphElement = NULL;

	if(strcmp(name, "cuadrado") == 0)
	{
		Square * square = new Square();
		square->SetPosition(GetPosition(tag, POSITION_TAG_NAME));
		square->SetColor(GetColor(tag, FIGURE_COLOR_ATT_NAME));

		TagProperty * sideAtt =  tag->GetAttribute("lado");

		if(sideAtt == NULL)
		{
			throw Exception("Side size missing for square figure.");
		}

		square->SetSide(atoi(sideAtt->GetValue()));
		
		graphElement = square;
	}
	else if(strcmp(name, "circulo") == 0)
	{
		Circle * circle = new Circle();
		circle->SetPosition(GetPosition(tag, POSITION_TAG_NAME));
		circle->SetColor(GetColor(tag, FIGURE_COLOR_ATT_NAME));

		TagProperty * radiusAtt =  tag->GetAttribute("radio");

		if(radiusAtt == NULL)
		{
			throw Exception("Radius missing for circle figure.");
		}

		circle->SetRadius(atoi(radiusAtt->GetValue()));

		graphElement = circle;
	}
	else if(strcmp(name, "rectangulo") == 0)
	{
		Rectangle * rectangle = new Rectangle();
		rectangle->SetPosition(GetPosition(tag, POSITION_TAG_NAME));
		rectangle->SetColor(GetColor(tag, FIGURE_COLOR_ATT_NAME));

		TagProperty * widthAtt =  tag->GetAttribute("base");

		if(widthAtt  == NULL)
		{
			throw Exception("Width missing for rectangle figure.");
		}

		rectangle->SetWidth(atoi(widthAtt->GetValue()));

		TagProperty * heightAtt =  tag->GetAttribute("altura");

		if(heightAtt  == NULL)
		{
			throw Exception("Height missing for rectangle figure.");
		}

		rectangle->SetHeight(atoi(heightAtt->GetValue()));

		graphElement = rectangle;
	}
	else if(strcmp(name, "segmento") == 0)
	{
		Line * line = new Line();
		line->SetBegin(GetPosition(tag, BEGIN_TAG_NAME));
		line->SetEnd(GetPosition(tag, END_TAG_NAME));
		line->SetColor(GetColor(tag, LINE_COLOR_ATT_NAME));

		graphElement = line;
	}
	else
	{
		throw Exception("Unknown graphic figure.");
	}

	return graphElement;
}

Position ModelValidator::GetPosition(Tag * tag, char * posTagName)
{
	Position pos;

	Tag * posicion = tag->GetChildTag(posTagName);

	if(posicion == NULL)	
	{
		throw Exception("Posicion missing.");
	}

	TagProperty * x = posicion->GetAttribute("x");
	TagProperty * y = posicion->GetAttribute("y");

	if(x == NULL || y == NULL)
	{
		throw Exception("x or y attributes missing.");
	}

	pos.x = atoi(x->GetValue());
	pos.y = atoi(y->GetValue());

	return pos;
}

Color ModelValidator::GetColor(Tag * tag, char * colorAttName)
{
	Color color;

	TagProperty * att = tag->GetAttribute(colorAttName);

	if(att != NULL)
	{
		char * RGB = att->GetValue();

		if(strlen(RGB) != 9)
		{
			throw Exception("Invalid format for color attribute.");
		}

		char * Red = StringHelper::Substring(RGB, 0, 3);
		char * Green = StringHelper::Substring(RGB, 3, 3);
		char * Blue = StringHelper::Substring(RGB, 6, 3);

		color.R = atoi(Red);
		color.G = atoi(Green);
		color.B= atoi(Blue);

		delete Red;
		delete Green;
		delete Blue;
	}
	else
	{
		color = ModelValidator::GetDefaultColor();
	}

	return color;
}

#endif