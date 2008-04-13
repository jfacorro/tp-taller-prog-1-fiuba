#include <stdio.h>
#include <fstream.h>
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
		char * source;

		Tag * BuildTree(ArrayList * stringsArr, Tag * parentTag, Tag * currentTag);
	public:
		char * getSource() { return this->source; };
		void setSource(char * sourceLocal) { this->source = sourceLocal; };
	
		XMLParser() {};

		Tag * ParseFile(char * filename);
		char * GetNextText(FILE * file);
		char * GetNextTag(FILE * file);

		static bool ValidateName(char * nameStr);
};

#endif