#ifndef __XML_DICTIONARY_PARSER_INCLUDED
#define __XML_DICTIONARY_PARSER_INCLUDED

#include "XmlParser.h"

struct XmlDictionaryNode
{
	int nodeId;
	string nodeName;
	map<string,string> nodeAttrs;

	XmlDictionaryNode* parent;
	XmlDictionaryNode* children;
	XmlDictionaryNode* next;
};

#define XML_DICTIONARY_PARSER_DEBUG_OUTPUT

class XmlDictionaryParser : public XmlParser
{
public:
	XmlDictionaryParser(string text);
	XmlDictionaryNode* GetRoot();

protected:
	virtual void OnParseStart();
	virtual void OnParseEnd();

	virtual void OnNodeStart(int nodeId, string nodeName,map<string,string> attributes);
	virtual void OnNodeEnd  (int nodeId, string nodeName);

	virtual void OnParseError(XmlParserErrors error, size_t pos);

private:
	XmlDictionaryNode root;
	XmlDictionaryNode* current;
};

#endif
