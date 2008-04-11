#include "XmlParser.h"

XmlParser::XmlParser(string text)
{
	this->text = text;
	this->pos = 0;
}

XmlParser::~XmlParser()
{
}


#define ISSTART(x)			(x == '<')
#define ISEND(x)			(x == '>')
#define ISSLASH(x)			(x == '/')
#define ISEXCLAMATION(x)	(x == '!')

#define ISBLANK(x)	(text[pos] == ' '  || text[pos] == '\t' || \
					 text[pos] == '\r' || text[pos] == '\n')

#define ISNAME(x)	((text[pos] >= 'a' && text[pos] <= 'z') || \
					 (text[pos] >= 'A' && text[pos] <= 'Z') || \
					 (text[pos] >= '0' && text[pos] <= '9') || \
					 text[pos] == '_' || text[pos] == '.' || text[pos] == '_')

XmlParserState XmlParser::__SkippingBlanks()
{
	XmlParserState retval = SkippingBlanks;
	
	if ( text[pos] == '<' )
		retval = ParsingName;
	else if ( !ISBLANK(text[pos]) )
	{
		OnParseError(BlankExpected,pos);				
		retval = ErrorExit;
	}

	return retval;
}

XmlParserState XmlParser::__ParsingName()
{
	XmlParserState retval = ParsingName;

	bool _continue = true;
	if ( nodeName.empty() && isNodeEnd == false)
	{
		if ( ISSLASH(text[pos]) ) 
		{
			isNodeEnd = true;
			_continue = false;
		}
		else if ( ISEXCLAMATION(text[pos]) )
		{
			_continue = false;

			if ( (pos + 2) >= text.size() )
			{
				OnParseError(UnexpectedEOF,pos);
				retval = ErrorExit;
			}
			else if ( text[pos+1] != '-' || text[pos+2] != '-' )
			{
				OnParseError(BadCommentStart,pos);
				retval = ErrorExit;
			}
			else
			{
				pos += 2;
				retval = Comment;
			}
		}
		else
			isNodeEnd = false;
	}

	if ( _continue )
	{
		if ( ISNAME(text[pos]) )
			nodeName += text[pos];
		else if ( ISBLANK(text[pos]) )
			retval = ParsingAttributes;
		else if ( ISEND(text[pos]) || ISSLASH(text[pos]) )
			retval = __OnTagEnd();
		else
		{
			OnParseError(ValidCharacterExpected,pos);
			retval = ErrorExit;
		}
	}

	return retval;
}

XmlParserState XmlParser::__AttributeParser()
{
	XmlParserState retval = ParsingAttributes;
	string attrName;
	string attrValue;

	while ( pos < text.size() && ISNAME(text[pos]) )
	{
		attrName += text[pos];
		pos++;
	}

	if ( (pos + 2) >= text.size () )
	{
		OnParseError(UnexpectedEOF,pos);
		retval = ErrorExit;
	}
	if ( text[pos] != '=' )
	{
		OnParseError(ValidCharacterExpected,pos);
		retval = ErrorExit;
	}
	else
	{
		pos++;
		if ( text[pos] != '\'' && text[pos] != '\"' )
		{
			OnParseError(ValidCharacterExpected,pos);
			retval = ErrorExit;
		}
		else
		{
			char start = text[pos];
			pos++;
			while ( pos < text.size() && text[pos] != start )
			{
				attrValue += text[pos];
				pos++;
			}

			if ( pos >= text.size () )
			{
				OnParseError(UnexpectedEOF,pos);
				retval = ErrorExit;
			}	
			else
			{
				map<string,string>::iterator iter = nodeAttrs.find(attrName);
				if ( iter == nodeAttrs.end() )
					nodeAttrs.insert(make_pair(attrName,attrValue));
				else
				{
					OnParseError(RedundantAttribute,pos);
					retval = ErrorExit;
				}
			}
		}
	}

	return retval;
}

XmlParserState XmlParser::__ParsingAttributes()
{
	XmlParserState retval = ParsingAttributes;
	
	if ( ISEND(text[pos]) || ISSLASH(text[pos]) )
		retval = __OnTagEnd();
	else if ( !ISBLANK(text[pos]) )
	{
		if ( isNodeEnd )
		{
			OnParseError(UnexpectedAttribute,pos);
			retval = ErrorExit;
		}
		else if ( !ISNAME(text[pos]) )
		{
			OnParseError(ValidCharacterExpected,pos);
			retval = ErrorExit;
		}
		else
			retval = __AttributeParser();
	}

	return retval;
}

XmlParserState XmlParser::__OnTagEnd()
{
	XmlParserState retval = SkippingBlanks;

	if ( isNodeEnd )
	{
		if ( ISSLASH(text[pos]) )
		{
			OnParseError(UnexpectedSlash,pos);
			retval = ErrorExit;
		}
		else
		{
			XmlNode* tmp = nodes.back();
			nodes.pop_back();
			if ( nodeName == tmp->nodeName )
			{
				OnNodeEnd(tmp->nodeId,nodeName);
				retval = SkippingBlanks;
			}
			else
			{
				OnParseError(UnmatchedTagEnd,pos);
				retval = ErrorExit;
			}

			delete tmp;
		}
	}
	else
	{
		bool recurse = false;
		if ( ISSLASH(text[pos]) )
		{
			if ( (pos+1) >= text.size() )
			{
				OnParseError(UnexpectedEOF,pos);
				retval = ErrorExit;
			}
			else if ( !ISEND(text[pos+1]) )
			{
				OnParseError(ValidCharacterExpected,pos);
				retval = ErrorExit;
			}
			else
				recurse = true;
		}

		if ( retval != ErrorExit )
		{
			OnNodeStart(nodeId,nodeName,nodeAttrs);

			XmlNode* tmp = new XmlNode();
			tmp->nodeId = nodeId;
			tmp->nodeName = nodeName;
			nodes.push_back(tmp);

			nodeId++;
		}

		if ( !recurse )
			retval = SkippingBlanks;
		else
		{
			pos++;
			isNodeEnd = true;
			retval = __OnTagEnd();
		}
	}

	isNodeEnd = false;
	nodeName.clear();
	nodeAttrs.clear();
	return retval;
}

XmlParserState XmlParser::__Comment()
{
	if ( ISEND(text[pos]) )
		if ( pos >= 2 && text[pos-1] == '-' &&
			 text[pos-2] == '-' )
		{
			if ( pos >= 3 && text[pos-3] == '-' )
			{
				OnParseError(BadCommentEnd,pos);
				return ErrorExit;
			}
			else
				return SkippingBlanks;
		}

	return Comment;
}

void XmlParser::Parse()
{
	XmlParserState state = SkippingBlanks;
	isNodeEnd = false;
	nodeId=1;

	OnParseStart();
	while ( state != ErrorExit && pos < text.size() )
	{	
		if ( state == SkippingBlanks )
			state = __SkippingBlanks();
		else if ( state == ParsingName )
			state = __ParsingName();
		else if ( state == ParsingAttributes )
			state = __ParsingAttributes();
		else if ( state == Comment )
			state = __Comment();

		pos++;
	}

	if ( state == SkippingBlanks )
	{
		if ( !nodes.empty() )
			OnParseError(OpenTag,pos);
	}
	else if ( state != ErrorExit )
		OnParseError(UnexpectedEOF,pos);

	nodes.clear();
	OnParseEnd();
}
