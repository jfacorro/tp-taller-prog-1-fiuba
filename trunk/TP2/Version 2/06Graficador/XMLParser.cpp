#include <stdio.h>
#include <stdlib.h>
#include <fstream.h>
#include "Common.h"
#include "XMLParser.h"
#include "Exception.h"
#include "Stack.h"
#include "Tag.h"
#include "TagProperty.h"

#ifndef XMLParser_cpp

#define XMLParser_cpp

Tag * XMLParser::ParseFile(char * filename)
{
	Tag * rootTag = NULL;

	FILE * file;
	
	file = fopen(filename, "r");

	if(file != NULL)
	{
		char * tagStr = NULL;
		char * textStr = NULL;

		ArrayList stringsArr;
		ArrayList tagsArr;

		do
		{
			textStr = this->GetNextText(file);

			if(textStr != NULL)
			{
				stringsArr.Add(textStr);
				/// printf("FOUND TEXT: %s\n", textStr);
			}

			tagStr = this->GetNextTag(file);

			if(tagStr != NULL)
			{
				stringsArr.Add(tagStr);
				/// printf("FOUND TAG: %s\n", tagStr);
				
			}
		}while(!feof(file));
		
		fclose(file);

		rootTag = this->BuildTree(&stringsArr, NULL, NULL);
	}
 	else
	{
		throw FileNotFoundException();
	}

	return rootTag;
}

/**************************************************************************
M�todo recursivo que construye el arbol XML de tags.
**************************************************************************/
Tag * XMLParser::BuildTree(ArrayList * stringsArr, Tag * parentTag, Tag * currentTag )
{
	char * currentStr = NULL;
	
	bool currentTagIsClosed = false;

	if(parentTag == NULL)
	{
		stringsArr->MoveFirst();		
	}
	
	while(!stringsArr->IsEmpty() && !currentTagIsClosed)
	{
		char * str = (char *)stringsArr->RemoveCurrent();

		/// Comprobar si la cadena es texto o un tag.
		if(str != NULL && str[0] == '<')
		{
			Tag * tag = new Tag(str);

			/// Si es un tag auto contenido
			/// no necesita del tag de cierre.
			if(tag->GetIsAutoClosed())
			{
				if(currentTag == NULL)
				{
					currentTag = tag;
					currentTagIsClosed = true;
				}
				else
				{
					currentTag->AddChildTag(tag);
				}
			}
			/// Si es un tag de cierre
			/// verificar que coincida con el de apertura
			/// si es que ya se cargo uno.
			else if(tag->GetIsClosingTag())
			{
				if(currentTag != NULL && strcmp(currentTag->GetName(), tag->GetName()) == 0 )
				{
					currentTagIsClosed = true;
				}
				else
				{
					throw NotWellFormedException();
				}
			}
			else if(currentTag == NULL)
			{
				currentTag = tag;
			}
			else
			{
				Tag * childTag = this->BuildTree(stringsArr, currentTag, tag);

				currentTag->AddChildTag(childTag);
			}
		}
		else if(str != NULL)
		{
			/// Si hay un tag agregarle 
			/// el texto.
			if(currentTag != NULL)
			{
				currentTag->SetInnerText(str);
			}
			else if(parentTag != NULL)
			{
				parentTag->SetInnerText(str);
			}
			/// Si no hay un tag padre entonces hay texto 
			/// afuera del root element, generar una excepcion.
			else
			{
				throw NotWellFormedException();
			}
		}
	}

	if(currentTagIsClosed)
	{
		if(parentTag == NULL && !stringsArr->IsEmpty())
		{
			throw RootElementMissingException();
		}

		return currentTag;
	}
	else
	{
		throw NotWellFormedException();
	}

	return NULL;
}

char * XMLParser::GetNextTag(FILE * file)
{
	char openChar;
	char closeChar;
	int openPos = 0;
	int closePos = 0;	

	char * tagStr = NULL;

	/// Los caracteres '\n' son contados de forma
	/// distinta, cada vez que se extrae uno, el puntero get
	/// aumenta en dos en realidad su indice.
	int numNewLineChar = 0;

	do
	{
		openChar = fgetc(file);

		if(openChar == '\n') numNewLineChar++;

		if(openChar == '<')
		{
			openPos = ftell(file) - (1 + numNewLineChar);
		}

	}while(!feof(file) && openChar != '<');

	numNewLineChar = 0;

	do
	{
		closeChar = fgetc(file);

		if(closeChar == '\n') numNewLineChar++;

		if(closeChar == '>')
		{
			closePos = ftell(file) - (1 + numNewLineChar);
		}
		else if (closeChar == '<')
		{
			throw ClosingTagMissingException();
		}
	}while(!feof(file) && closeChar != '>');


	if(closePos != 0 && !feof(file))
	{
		fseek(file, openPos, SEEK_SET);

		tagStr = new char[closePos - openPos + 2];

		fread(tagStr, 1, closePos - openPos + 1, file);

		tagStr[closePos - openPos + 1] = '\0';

		fseek(file, closePos + 1, SEEK_SET);

		char * temp = tagStr;
		
		tagStr = StringHelper::Trim(temp);

		delete temp;
	}

	return tagStr;
}

char * XMLParser::GetNextText(FILE * file)
{
	char openChar;
	int beginPos = ftell(file);
	int endPos = 0;	
	int openCharPos = 0;	

	char * tagStr = NULL;

	int numNewLineChar = 0;

	do
	{
		openChar = fgetc(file);

		if(openChar == '\n') numNewLineChar++;

		if(openChar == '<')
		{
			openCharPos = ftell(file) - (1 + numNewLineChar);
			
			endPos = openCharPos;

			if(beginPos != endPos)
			{
				endPos--;
			}			
		}

	}while(!feof(file) && openChar != '<');	

	if(openCharPos != beginPos && !feof(file))
	{
		tagStr = new char[endPos - beginPos + 2];

		fseek(file, beginPos, SEEK_SET);

		fread(tagStr, 1, endPos - beginPos + 1, file);

		fseek(file, endPos + 1, SEEK_SET);

		tagStr[endPos - beginPos + 1] = '\0';

		char * temp = tagStr;
		
		tagStr = StringHelper::Trim(temp);

		delete temp;

		if(tagStr[0] == '\0')
		{
			delete tagStr;
			tagStr = NULL;
		}
	}
	else
	{
		fseek(file, beginPos, SEEK_SET);
	}


	return tagStr;
}

bool XMLParser::ValidateName(char * nameStr)
{
	int length = strlen(nameStr);

	/// Verificar que no tiene ninguna ".
	int quoteIndex = StringHelper::FindFirstOcurrence(nameStr, length, 0, '"');

	if(quoteIndex != -1)
	{
		return false;
	}

	/// Verificar que no tiene ningun &.
	int ampersandIndex = StringHelper::FindFirstOcurrence(nameStr, length, 0, '&');

	if(ampersandIndex != -1)
	{
		return false;
	}

	/// Verificar que el primer caracter
	/// no sea un n�mero.
	int numChar = (int)nameStr[0];

	if(numChar >= (int)'0' && numChar <= (int)'9')
	{
		return false;
	}

	return true;
}

#endif