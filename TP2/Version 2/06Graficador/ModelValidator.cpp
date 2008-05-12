#include "iostream.h"
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

			GetGraphicElement(child);

		}while(children.MoveNext());
	}
}

void ModelValidator::GetGraphicElement(Tag * tag)
{
 	char * name = tag->GetName();

	GraphicElement * graphElement = NULL;

	if(strcmp(name, "General") == 0)
	{
		if(!this->config.GetIsDefaultConfig())
		{
			throw Exception("There can only be one general element.");
			
		}
		
		Tag * resTag = tag->GetChildTag("resolucion");

		if(resTag != NULL)
		{
			char * resStr = resTag->GetInnerText();
			int resInt = atoi(resStr);

			this->config.SetResolucion(SDLHelper::ResolutionByWidth(resInt));
		}

		Tag * colorFondoGrafTag = tag->GetChildTag("colorFondoGraf");

		if(colorFondoGrafTag != NULL)
		{
			char * colorFondoGrafStr = colorFondoGrafTag->GetInnerText();

			Color colorFondoGraf = ModelValidator::GetColorFromString(colorFondoGrafStr);

			this->config.SetColorFondoGraf(colorFondoGraf);
		}


		Tag * texturaTag = tag->GetChildTag("textura");

		if(texturaTag != NULL)
		{
			char * texturaStr = texturaTag->GetInnerText();

			this->config.SetTextura(GetTextureById(texturaStr));
		}

		Tag * colorLineaTag = tag->GetChildTag("colorLinea");

		if(colorLineaTag != NULL)
		{
			char * colorLineaStr = colorLineaTag->GetInnerText();

			Color colorLinea = ModelValidator::GetColorFromString(colorLineaStr);

			this->config.SetColorLinea(colorLinea);
		}


		Tag * colorFondoTag = tag->GetChildTag("colorFondo");

		if(colorFondoTag != NULL)
		{
			char * colorFondoStr = colorFondoTag->GetInnerText();

			Color colorFondo = ModelValidator::GetColorFromString(colorFondoStr);

			this->config.SetColorFondo(colorFondo);
		}
	}
	else if(strcmp(name, "cuadrado") == 0)
	{
		Square * square = new Square();
		square->SetId(GetId(tag));
		square->SetPosition(GetPosition(tag, POSITION_TAG_NAME));
		square->SetColor(GetColor(tag, FIGURE_COLOR_ATT_NAME));

		TagProperty sideAtt =  tag->GetAttribute("lado");

		if(sideAtt.GetName() == NULL)
		{
			throw Exception("Side size missing for square figure.");
		}

		square->SetSide(atoi(sideAtt.GetValue()));

		square->SetTextura(GetTexture(tag));
		
		graphElement = square;
	}
	else if(strcmp(name, "circulo") == 0)
	{
		Circle * circle = new Circle();
		circle->SetId(GetId(tag));
		circle->SetPosition(GetPosition(tag, POSITION_TAG_NAME));
		circle->SetColor(GetColor(tag, FIGURE_COLOR_ATT_NAME));

		TagProperty radiusAtt =  tag->GetAttribute("radio");

		if(radiusAtt.GetName() == NULL)
		{
			throw Exception("Radius missing for circle figure.");
		}

		circle->SetRadius(atoi(radiusAtt.GetValue()));

		circle->SetTextura(GetTexture(tag));

		graphElement = circle;
	}
	else if(strcmp(name, "rectangulo") == 0)
	{
		Rectangle * rectangle = new Rectangle();
		rectangle->SetId(GetId(tag));
		rectangle->SetPosition(GetPosition(tag, POSITION_TAG_NAME));
		rectangle->SetColor(GetColor(tag, FIGURE_COLOR_ATT_NAME));

		TagProperty widthAtt =  tag->GetAttribute("base");

		if(widthAtt.GetName() == NULL)
		{
			throw Exception("Width missing for rectangle figure.");
		}

		rectangle->SetWidth(atoi(widthAtt.GetValue()));

		TagProperty heightAtt = tag->GetAttribute("altura");

		if(heightAtt.GetName() == NULL)
		{
			throw Exception("Height missing for rectangle figure.");
		}

		rectangle->SetHeight(atoi(heightAtt.GetValue()));

		/// Esto tiene que ser asignado despues de la 
		/// altura y el ancho asi hace el reisze sólo una vez.
		rectangle->SetTextura(GetTexture(tag));

		graphElement = rectangle;
	}
	else if(strcmp(name, "segmento") == 0)
	{
		Line * line = new Line();
		line->SetId(GetId(tag));
		line->SetBegin(GetPosition(tag, BEGIN_TAG_NAME));
		line->SetEnd(GetPosition(tag, END_TAG_NAME));
		line->SetColor(GetColor(tag, LINE_COLOR_ATT_NAME));

		graphElement = line;
	}
	else if(strcmp(name, "textura") == 0)
	{
		Texture * texture = new Texture();
		texture->SetId(GetId(tag));

		TagProperty pathAtt = tag->GetAttribute("path");

		if(pathAtt.GetName() == NULL)
		{
			throw Exception("Path missing for texture.");
		}

		SDL_Surface * bitmap = SDLHelper::LoadBitmap(pathAtt.GetValue());

		texture->SetFilePath(pathAtt.GetValue());

		if(bitmap == NULL)
		{
			throw Exception("Could not load bitmap.");
		}

		texture->SetBitmap(bitmap);

		/// cout << "(Ancho -Alto) textura '" << texture->GetId() << "':" << texture->GetBitmap()->clip_rect.w << " - " << texture->GetBitmap()->clip_rect.h << endl;

		if(CheckDuplicatedId(&textures, texture->GetId()))
		{
			throw Exception("Duplicated texture id.");
		}
		else
		{
			this->textures.Add(texture);
		}
	}
	else
	{
		throw Exception("Unknown graphic figure.");
	}

	if(graphElement != NULL)
	{	
		bool existingId = CheckDuplicatedId(&graphicElements, graphElement->GetId());

		if(existingId)
		{
			throw Exception("Duplicated graphic element id.");
		}
		else
		{
			this->graphicElements.Add(graphElement);
		}
	}
}

bool ModelValidator::CheckDuplicatedId(ArrayList * graphElementsArr, char * idStr)
{
	bool existingId = false;

	if(!graphElementsArr->IsEmpty())
	{

		graphElementsArr->MoveFirst();

		do
		{
			GraphicElement * graphElementId = (GraphicElement *)graphElementsArr->GetCurrent();

			if(strcmp(graphElementId->GetId(), idStr) == 0)
			{
				existingId = true;
				break;
			}				

		}while(graphElementsArr->MoveNext() && !existingId);
	}

	return existingId;
}

char * ModelValidator::GetId(Tag * tag)
{
	char * id = NULL;

	TagProperty idAtt = tag->GetAttribute("id");

	if(idAtt.GetName() == NULL)	
	{
		throw Exception("Id missing.");
	}

	char * idAttValue = idAtt.GetValue();

	return idAttValue;
}

SDL_Surface * ModelValidator::GetTexture(Tag * tag)
{
	SDL_Surface * bitmap = NULL;

	TagProperty textureId = tag->GetAttribute("textura");

	if(textureId.GetName() != NULL)
	{
		bitmap = GetTextureById(textureId.GetValue());
	}

	return bitmap;
}

SDL_Surface * ModelValidator::GetTextureById(char * textureId)
{
	SDL_Surface * bitmap = NULL;

	if(!this->textures.IsEmpty())
	{
		this->textures.MoveFirst();

		do
		{
			Texture * texture = (Texture *)this->textures.GetCurrent();

			if(strcmp(texture->GetId(), textureId) == 0)
			{
				bitmap = SDLHelper::LoadBitmap(texture->GetFilePath());
				break;
			}

		}while(this->textures.MoveNext());
	}

	return bitmap;
}

Position ModelValidator::GetPosition(Tag * tag, char * posTagName)
{
	Position pos;

	Tag * posicion = tag->GetChildTag(posTagName);

	if(posicion == NULL)	
	{
		throw Exception("Posicion missing.");
	}

	TagProperty x = posicion->GetAttribute("x");
	TagProperty y = posicion->GetAttribute("y");

	if(x.GetName() == NULL || y.GetName() == NULL)
	{
		throw Exception("x or y attributes missing.");
	}

	pos.x = atoi(x.GetValue());
	pos.y = atoi(y.GetValue());

	return pos;
}

Color ModelValidator::GetColor(Tag * tag, char * colorAttName)
{
	Color color;

	TagProperty att = tag->GetAttribute(colorAttName);

	if(att.GetName() != NULL)
	{
		char * RGB = att.GetValue();

		color = ModelValidator::GetColorFromString(RGB);
	}
	else
	{
		color = SDLHelper::GetDefaultFrontColor();
	}

	return color;
}

Color ModelValidator::GetColorFromString(char * RGB)
{
	Color color;

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

	return color;
}

#endif