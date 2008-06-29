#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "Common.h"
#include "XMLParser.h"
#include "Exception.h"
#include "Tag.h"
#include "TagProperty.h"

#ifndef XMLParser_cpp

#define XMLParser_cpp

Tag * XMLParser::ParseFile(char * filename)
{
	Tag * rootTag = NULL;

	FILE * file;
	
	file = fopen(filename, "rb");


	if(file != NULL)
	{
		long filesize = 0;

		fseek(file, 0, SEEK_END);		
		filesize = ftell(file);

		char * source = new char[filesize + 1];
		//char * source = (char *)malloc(sizeof(char) * filesize);

		fseek(file, 0, SEEK_SET);
		fread(source, sizeof(char), filesize, file);

		fclose(file);

		source[filesize] = '\0';

		rootTag = this->Parse(source);

		free(source);
	}
 	else
	{
		throw FileNotFoundException();
	}

	return rootTag;
}

Tag * XMLParser::Parse(char * stringXml)
{
	Tag * rootTag = NULL;

	this->stringXml = stringXml;
	this->stringLength = strlen(stringXml) + 1;
	
	char * tagStr = NULL;
	char * textStr = NULL;

	ArrayList stringsArr;
	ArrayList tagsArr;

	do
	{
		textStr = this->GetNextText();

		if(textStr != NULL)
		{
			stringsArr.Add(textStr);
			/// printf("FOUND TEXT: %s\n", textStr);
		}

		tagStr = this->GetNextTag();

		if(tagStr != NULL)
		{
			stringsArr.Add(tagStr);
			/// printf("FOUND TAG: %s\n", tagStr);
			
		}
	}while(this->stringIndex < this->stringLength);
	
	rootTag = this->BuildTree(&stringsArr, NULL, NULL);

	return rootTag;
}

/**************************************************************************
Método recursivo que construye el arbol XML de tags.
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
					throw NotWellFormedException(tag->GetName());
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
				throw NotWellFormedException("There's text outside the root element.");
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
	/// No hay tag de cierre.
	else
	{
		throw NotWellFormedException(StringHelper::AppendString("There's a closing tag missing for ",  currentTag->GetName()));
	}

	return NULL;
}

char * XMLParser::GetNextTag()
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
		openChar = this->stringXml[this->stringIndex];

		this->stringIndex++;

		if(openChar == '\n')
		{
			numNewLineChar++;
		}

		if(openChar == '<')
		{
			openPos = this->stringIndex - (1 + numNewLineChar);
		}

	}while(this->stringIndex < this->stringLength && openChar != '<');

	numNewLineChar = 0;

	do
	{
		closeChar = this->stringXml[this->stringIndex];

		this->stringIndex++;

		if(closeChar == '\n')
		{
			numNewLineChar++;
		}

		if(closeChar == '>')
		{
			closePos = this->stringIndex - (1 + numNewLineChar);
		}
		else if (closeChar == '<')
		{
			throw ClosingTagMissingException();
		}
	}while(this->stringIndex <= this->stringLength && closeChar != '>');


	if(closePos != 0 && this->stringIndex <= this->stringLength)
	{
		this->stringIndex = openPos;

		tagStr = new char[closePos - openPos + 2];

		tagStr = StringHelper::Substring(this->stringXml, this->stringIndex, closePos - openPos + 1);

		tagStr[closePos - openPos + 1] = '\0';

		this->stringIndex = closePos + 1;

		char * temp = tagStr;
		
		tagStr = StringHelper::Trim(temp);

		delete temp;
	}

	return tagStr;
}

char * XMLParser::GetNextText()
{
	char openChar;
	int beginPos = this->stringIndex;
	int endPos = beginPos;	
	int openCharPos = 0;	

	char * tagStr = NULL;

	int numNewLineChar = 0;

	do
	{
		openChar = this->stringXml[this->stringIndex];

		this->stringIndex++;

		if(openChar == '\n')
		{
			numNewLineChar++;
		}

		if(openChar == '<')
		{
			openCharPos = this->stringIndex - (1 + numNewLineChar);
			
			endPos = openCharPos;

			if(beginPos != endPos)
			{
				endPos--;
			}			
		}

	}while(this->stringIndex < this->stringLength && openChar != '<');	

	if(openCharPos != beginPos && this->stringIndex <= this->stringLength)
	{
		tagStr = new char[endPos - beginPos + 2];

		this->stringIndex  = beginPos;

		tagStr = StringHelper::Substring(this->stringXml, this->stringIndex, endPos - beginPos + 1);

		this->stringIndex = endPos + 1;

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
		this->stringIndex = beginPos;
		/// fseek(file, beginPos, SEEK_SET);
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
	/// no sea un número.
	int numChar = (int)nameStr[0];

	if(numChar >= (int)'0' && numChar <= (int)'9')
	{
		return false;
	}

	return true;
}

#endif
