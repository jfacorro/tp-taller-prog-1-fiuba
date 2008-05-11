#include <string.h>
#include "StringHelper.h"

#ifndef Exception_h

#define Exception_h

class Exception
{
	public:
		/// Constructors ///
		Exception(char * msg) 
		{
			this->message = NULL;
			this->SetMessage(msg);
		};

		Exception() 
		{
			this->message = NULL;
		};

		/// Accessors ///

		void SetMessage(char * msg)
		{
			if(this->message != NULL)
				delete this->message;

			this->message = new char[strlen(msg)];

			strcpy(this->message, msg);
		};

		char * GetMessage() const
		{
			return this->message;
		};
	protected:
		char * message;
};

class FileNotFoundException : public Exception
{
	public:
		FileNotFoundException()
		{
			this->SetMessage("The file was not found.");
		};
};

class ClosingTagMissingException : public Exception
{
	public:
		ClosingTagMissingException()
		{
			this->SetMessage("There's a closing tag missing.");
		};
};


class NotWellFormedException : public Exception
{
	public:
		NotWellFormedException()
		{
			this->message = NULL;

			this->SetMessage("XML not well formed.");
		};

		NotWellFormedException(char * msg)
		{
			char * xmlWellFormedMsg = "XML not well formed. ";

			xmlWellFormedMsg = StringHelper::AppendString(xmlWellFormedMsg, msg);

			this->SetMessage(xmlWellFormedMsg);
		};
};

class RootElementMissingException : public Exception
{
	public:
		RootElementMissingException()
		{
			this->SetMessage("Root element missing.");
		};
};

class NameNotValidException : public Exception
{
	public:
		NameNotValidException(char * name)
		{
			char msgStr[150];
			strcpy(msgStr, "Invalid name: [");
			strcat(msgStr, name);
			strcat(msgStr, "].\n");

			this->SetMessage(msgStr);
		};
};

#endif