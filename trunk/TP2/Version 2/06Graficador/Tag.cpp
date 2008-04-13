#include <stdio.h>
#include <string.h>
#include "Tag.h"
#include "Exception.h"
#include "StringHelper.h"
#include "TagProperty.h"
#include "XMLParser.h"

#ifndef Tag_cpp

#define Tag_cpp

#define BLANK_SPACE ' '

void Tag::ParseTagString(char * tagString)
{
	/// Reemplazar todos los caracteres newline (\n), tab (\t) y
	/// return carrier(\r) por un espacio en blanco.
	for(int index = 0; index < strlen(tagString); index++)
	{
		if(tagString[index] == '\n') tagString[index] = BLANK_SPACE;
		else if(tagString[index] == '\t') tagString[index] = BLANK_SPACE;
		else if(tagString[index] == '\r') tagString[index] = BLANK_SPACE;
	}

	/// Verificar si es o no sea un tag de cierre.
	this->isClosingTag = (tagString[1] == '/');

	/// Verificar que el primer character despues del '<' 
	/// o del '</' no sea un espacio en blanco.
	if
	(
		(tagString[1] == BLANK_SPACE && !this->isClosingTag) ||
		(tagString[2] == BLANK_SPACE && this->isClosingTag)
	)
	{
		throw NotWellFormedException();
	}

	/// Verificar si esta o no cerrado en él mismo.
	this->isAutoClosed = (tagString[strlen(tagString) - 2] == '/');

	/// Extraer el nombre del tag
	this->name = this->ExtractName(tagString);

	if(!XMLParser::ValidateName(this->name))
	{
		throw NameNotValidException(this->name);
	}

	/// TODO: Validar el nombre (buscar caracteres invalidos para nombres).

	/// Parsear las propiedades si hay alguna
	this->ParseProperties(tagString);	
}

void Tag::ParseProperties(char * tagString)
{
	int length = strlen(tagString);
	int firstSpace = StringHelper::FindFirstOcurrence(tagString, length, 0, BLANK_SPACE);

	/// Si hay un espacio en el tag puede llegar a 
	/// haber alguna propiedad
	if(firstSpace != -1)
	{
		int index = firstSpace;

		char current = tagString[index];
		
		/// Iterar sobre todos los caracteres hasta
		/// encontrar alguno que no sea un espacio.
		do
		{
			current = tagString[index];

			if(current != BLANK_SPACE && current != '>')
			{
				/// Indice que indica donde comienza la propiedad
				int beginPropIndex = index;

				/// Si se encuetra un caracter diferente 
				/// de un espacio en un closing tag generar 
				/// una excepcion.
				if(this->isClosingTag)
				{
					throw NotWellFormedException();
				}
				/// Si es un AutoClosing tag verificar que 
				/// no sea la barra antes del '>' final
				else if
				(
					this->isAutoClosed &&	
					current == '/' &&
					index == length - 2
				)
				{
					break;
				}

				int firstEqual = StringHelper::FindFirstOcurrence(tagString, length, index, '=');

				/// Si no encontro un igual generar una excepcion.
				if(firstEqual == -1)
				{
					throw NotWellFormedException();
				}

				index = firstEqual  + 1;
				
				/// TODO: Contemplar el caso en el que el 
				/// valor esté entre comillas simples.
				int firstQuote = StringHelper::FindFirstOcurrence(tagString, length, index, '"');

				/// Si no encontro unas comillas de apertura
				/// generar una excepcion.
				if(firstQuote == -1)
				{
					throw NotWellFormedException();
				}

				index = firstQuote + 1;

				int secondQuote = StringHelper::FindFirstOcurrence(tagString, length, index, '"');

				/// Si no encontro unas comillas de cierre
				/// generar una excepcion.
				if(secondQuote == -1)
				{
					throw NotWellFormedException();
				}

				index = secondQuote + 1;
				
				current = tagString[index];

				int lengthPropStr = secondQuote - beginPropIndex + 1;

				char * propertyStr = StringHelper::Substring(tagString, beginPropIndex, lengthPropStr);

				TagProperty * property = new TagProperty(propertyStr);

				this->properties.Add(property);
			}
			else
			{
				index++;
			}			

		}while(index < length);
	}
}

char * Tag::ExtractName(char * tagString)
{
	char * strName;
	int firstSpace = 0;
	int startAtIndex = 1;
	int nameLength = 0;
	int length = strlen(tagString);
	
	firstSpace = StringHelper::FindFirstOcurrence(tagString, length, 0, BLANK_SPACE);

	if(firstSpace == -1)
	{
		if(this->isAutoClosed)
			firstSpace = length - 2;
		else
			firstSpace = length - 1;
	}

	if(this->isClosingTag)
	{
		startAtIndex = 2;
	}

	nameLength = firstSpace - startAtIndex;

	strName = StringHelper::Substring(tagString, startAtIndex, nameLength);

	return strName;
}

void Tag::Print()
{
	if(this->isClosingTag)
	{
		printf("</%s", this->name);
	}
	else
	{
		printf("<%s", this->name);
	}

	this->properties.MoveFirst();

	if(!this->properties.IsEmpty())
	{
		do
		{
			TagProperty * property = (TagProperty *)this->properties.GetCurrent();

			printf(" %s=\"%s\"", property->GetName(), property->GetValue());

		}while(this->properties.MoveNext());
	}

	if(this->isAutoClosed)
	{
		printf("/>\n");
	}
	else
	{
		printf(">\n");

		if(this->innerText != NULL)
			printf(this->innerText);

		if(!this->childTags.IsEmpty())
		{
			this->childTags.MoveFirst();

			do
			{
				Tag * tag = (Tag *)this->childTags.GetCurrent();

				tag->Print();
			}while(this->childTags.MoveNext());
		}

		printf("</%s>\n", this->name);
	}

}

#endif