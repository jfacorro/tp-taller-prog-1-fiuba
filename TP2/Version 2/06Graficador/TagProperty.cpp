#include "Tag.h"
#include "TagProperty.h"
#include "XMLParser.h"
#include "Exception.h"

#ifndef TagProperty_cpp

#define TagProperty_cpp

TagProperty::TagProperty()
{
	this->name = NULL;
	this->value = NULL;
}

TagProperty::TagProperty(char * name, char * value)
{
	this->name = StringHelper::Substring(name, 0, strlen(name));
	this->value = StringHelper::Substring(value, 0, strlen(value));
}

TagProperty::TagProperty(char * propStr)
{
	int length = strlen(propStr);
	int firstEqual = StringHelper::FindFirstOcurrence(propStr, length, 0, '=');

	this->name = StringHelper::Substring(propStr, 0, firstEqual);

	char * temp = this->name;
	
	this->name = StringHelper::Trim(temp);

	if(!XMLParser::ValidateName(this->name))
	{
		throw NameNotValidException(this->name);
	}

	delete temp;

	int firstQuote = StringHelper::FindFirstOcurrence(propStr, length, firstEqual + 1, '"');

	this->value = StringHelper::Substring(propStr, firstQuote + 1, length - (firstQuote + 1) - 1);
}

#endif