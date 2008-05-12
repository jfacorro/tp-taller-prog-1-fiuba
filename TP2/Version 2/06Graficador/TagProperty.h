#include "StringHelper.h"

#ifndef TagProperty_h

#define TagProperty_h

class TagProperty
{
	private:
		char * name;
		char * value;

	public:
		TagProperty(char * name, char * value);
		TagProperty(char * propStr);
		TagProperty();

		char * GetName() { return this->name; };		

		char * GetValue() { return this->value; };
		
		void SetValue(char * value) 
		{ 
			if(this->value != NULL)
				delete this->value;

			this->value = new char[strlen(value)];

			strcpy(this->value, value);
		};
};

#endif