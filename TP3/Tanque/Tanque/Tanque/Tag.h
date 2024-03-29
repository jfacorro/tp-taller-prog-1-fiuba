#include <string.h>
#include "Common.h"
#include "ArrayList.h"
#include "TagProperty.h"

#ifndef Tag_h

#define Tag_h

class Tag
{
	private:
		/// Properties
		bool isClosingTag;
		bool isAutoClosed;
		char * name;
		char * innerText;

		/// Methods
		void ParseTagString(char * tagString);
		char * ExtractName(char * tagString);		
		void ParseProperties(char * tagString);
		
	public:
		/// Constructor
		Tag(char * tagString)
		{
			this->isClosingTag = 0;
			this->isAutoClosed = 0;
			this->name = NULL;
			this->innerText = NULL;
			this->ParseTagString(tagString);
		};
        ~Tag();

		/// Accessors
		char * GetName() { return this->name; };
		char * GetInnerText() { return this->innerText; };
		void SetInnerText(char * innerText) 
		{
			if(this->innerText != NULL)
				delete this->innerText;

			this->innerText = StringHelper::Substring(innerText, 0, strlen(innerText));
		};
		bool GetIsClosingTag() { return this->isClosingTag; };
		bool GetIsAutoClosed() { return this->isAutoClosed; };

		/// Methods
		void AddChildTag(Tag * child) { this->childTags.Add(child); };
		void Print();

		/// Properties
		ArrayList properties;
		ArrayList childTags;

		TagProperty * GetAttribute(char * attName);
		Tag * GetChildTag(char * tagName);
		
};

#endif
