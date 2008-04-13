#include "stdio.h"
#include "XMLParser.h"
#include "Exception.h"

#define DEBUG 1

int main(int argc, char* argv[] ) 
{

	XMLParser parser;

	try
	{
		#ifdef DEBUG
		
		Tag * rootNode = parser.ParseFile("C:\\Juan\\Facultad\\2008.1er.Taller.de.Programacion.I\\TP Nº2 Google\\06Graficador\\test.xml");

		rootNode->Print();

		#else

		if(argc > 1)
		{
			char * fileName = argv[1];

			Tag * rootNode = parser.ParseFile(fileName);

			rootNode->Print();
		}
		else
		{
			printf("\nDebe ingresar el path del archivo XML.\n");
		}

		#endif
	}
	catch(Exception ex)
	{
		printf("\nAn Exception ocurred: %s", ex.GetMessage());
	}

	return 0;
}