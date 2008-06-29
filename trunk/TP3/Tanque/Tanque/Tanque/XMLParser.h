#include <stdio.h>
#include <fstream>
#include "ArrayList.h"
#include "Tag.h"

#ifndef XMLParser_h

#define XMLParser_h

enum ParserResult 
{	
	PARSER_OK , 
	PARSER_FILE_NOT_FOUND, 
	PARSER_CLOSING_TAG_MISSING
};

class XMLParser
{
	private: 
		int stringIndex;
		int stringLength;

		char * stringXml;

		Tag * BuildTree(ArrayList * stringsArr, Tag * parentTag, Tag * currentTag);
		char * GetNextText();
		char * GetNextTag();
	public:
		char * getSource() { return this->stringXml; };
		void setSource(char * sourceLocal) { this->stringXml = sourceLocal; };
	
		XMLParser() 
		{
			this->stringIndex = 0;
			this->stringLength = 0;
		};

		Tag * ParseFile(char * filename);
		Tag * Parse(char * stringXml);

		static bool ValidateName(char * nameStr);
};

#endif