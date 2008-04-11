#ifndef __XML_PARSER_INCLUDED
#define __XML_PARSER_INCLUDED

#include <string>
#include <list>
#include <map>

using namespace std;

typedef enum _XmlParserErrors
{
	BlankExpected,
	ValidCharacterExpected,
	UnmatchedTagEnd,
	OpenTag,
	BadCommentStart,
	BadCommentEnd,
	UnexpectedEOF,
	UnexpectedSlash,
	UnexpectedAttribute,
	RedundantAttribute
}
	XmlParserErrors;

typedef enum _XmlParserState
{
	SkippingBlanks,
	ParsingName,
	ParsingAttributes,
	Comment,
	ErrorExit
} 
	XmlParserState;

typedef struct _XmlNode
{
	int nodeId;
	string nodeName;
	map<string,string> attributes;
}
	XmlNode;

class XmlParser
{
public:
	XmlParser(string text);
	virtual ~XmlParser();
	void Parse();

protected:
	virtual void OnParseStart() = 0;
	virtual void OnParseEnd() = 0;

	virtual void OnNodeStart(int nodeId, string nodeName,map<string,string> attributes) = 0;
	virtual void OnNodeEnd  (int nodeId, string nodeName) = 0;

	virtual void OnParseError(XmlParserErrors error, size_t pos) = 0;

private:
	XmlParserState	__SkippingBlanks();
	XmlParserState	__ParsingName();
	XmlParserState  __ParsingAttributes();
	XmlParserState	__AttributeParser();
	XmlParserState	__OnTagEnd();
	XmlParserState	__Comment();

	string text;
	size_t pos;

	bool isNodeEnd;
	string nodeName;
	map<string,string> nodeAttrs;
	list<XmlNode*> nodes;
	int nodeId;
};

#endif
