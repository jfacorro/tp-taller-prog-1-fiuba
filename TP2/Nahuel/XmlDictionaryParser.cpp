#include "XmlDictionaryParser.h"
#include <iostream>

XmlDictionaryParser::XmlDictionaryParser(string text) 
	: XmlParser(text)
{
}

XmlDictionaryNode* XmlDictionaryParser::GetRoot()
{
	return root.children;
}


void XmlDictionaryParser::OnParseStart()
{
#ifdef XML_DICTIONARY_PARSER_DEBUG_OUTPUT
cout << "XmlDictionaryParser::OnParseStart()" << endl;
#endif

	root.parent = NULL;
	root.next = NULL;
	root.children = NULL;
	current = &root;
}

void XmlDictionaryParser::OnParseEnd()
{
#ifdef XML_DICTIONARY_PARSER_DEBUG_OUTPUT
cout << "XmlDictionaryParser::OnParseEnd()" << endl;
#endif
}

void XmlDictionaryParser::OnNodeStart(int nodeId, string nodeName,map<string,string> attributes)
{
#ifdef XML_DICTIONARY_PARSER_DEBUG_OUTPUT
	cout << "XmlDictionaryParser::OnNodeStart(" << nodeId << ",\"" << nodeName << "\",{";
	map<string,string>::iterator iter;
	for ( iter = attributes.begin() ; iter != attributes.end() ; ++iter )
		cout << "\"" << iter->first << "\":\"" << iter->second << "\",";
	cout << "})" << endl;
#endif

	XmlDictionaryNode* tmp;
	tmp = new XmlDictionaryNode();
	tmp->nodeId = nodeId;
	tmp->nodeName = nodeName;
	tmp->nodeAttrs = attributes;
	tmp->parent = current;
	tmp->children = NULL;
	tmp->next = NULL;

	XmlDictionaryNode** upd = &current->children;
	while ( *upd != NULL )
		upd = &(*upd)->next;

	*upd = tmp;
	current = tmp;
}

void XmlDictionaryParser::OnNodeEnd(int nodeId, string nodeName)
{
#ifdef XML_DICTIONARY_PARSER_DEBUG_OUTPUT
cout << "XmlDictionaryParser::OnNodeEnd(" << nodeId << ",\"" << nodeName << "\")" << endl;
#endif

	current = current->parent;
}

void XmlDictionaryParser::OnParseError(XmlParserErrors error, size_t pos)
{
#ifdef XML_DICTIONARY_PARSER_DEBUG_OUTPUT
cout << "XmlDictionaryParser::OnParseError(" << error << "," << pos << ")" << endl;
#endif

	root.children = NULL;
	// TODO: Limpiar el diccionario en caso de error
}
