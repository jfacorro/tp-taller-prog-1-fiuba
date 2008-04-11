// xml.cpp : Defines the entry point for the console application.
//

#include "XmlDictionaryParser.h"
#include <iostream>
//#include "sdl.h"








void PrintDictionary ( XmlDictionaryNode* p , string indent )
{
	cout << indent << "(" << p->nodeId << ") " << p->nodeName << " {";

	map<string,string>::iterator iter;
	for ( iter = p->nodeAttrs.begin() ; iter != p->nodeAttrs.end() ; ++iter )
		cout << "\"" << iter->first << "\":\"" << iter->second << "\",";
	
	cout << "}" << endl;

	XmlDictionaryNode* c = p->children;
	while ( c != NULL )
	{
		PrintDictionary ( c , indent + "  " );
		c = c->next;
	}
}


int main(int argc, char* argv[])
{
	/*
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		cout << "Unable to initialize SDL: " << SDL_GetError() << endl;
	}
	else
	{
	*/
		string xml = "<xml a='1234' b=\"asas231\" ><root>  <blg><crop/><crap /> </blg><!-- comme < nt > --></root><delta /><theta /></xml>";
		XmlDictionaryParser parser(xml);
		parser.Parse();

		XmlDictionaryNode* d = parser.GetRoot();
		PrintDictionary ( d , "" );
	/*
		SDL_Quit();
	}
	*/

	int k;
	cin >> k;
	return 0;
}

