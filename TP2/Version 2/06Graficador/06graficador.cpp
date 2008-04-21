#include "conio.h"
#include "stdio.h"
#include "XMLParser.h"
#include "Exception.h"
#include "SDL.h"
#include "SDLHelper.h"
#include "ModelValidator.h"

#define DEBUG 1

void TestSDL();
void TestSDLHelper();
void display_bmp(char *file_name, SDL_Surface *screen);

int main(int argc, char* argv[] ) 
{
	XMLParser parser;

	try
	{
		#ifdef DEBUG
		
		Tag * rootNode = parser.ParseFile("C:\\Juan\\Facultad\\2008.1er.Taller.de.Programacion.I\\TP Nº2\\Version 2\\06Graficador\\figuras.xml");

		rootNode->Print();

		ModelValidator modelVal;
		
		modelVal.ParseAndValidate(rootNode);

		SDLHelper sdlHelper;

		sdlHelper.Initialize(800, 600, 32);

		if(!modelVal.graphicElements.IsEmpty())
		{
			modelVal.graphicElements.MoveFirst();

			do
			{
				GraphicElement * graphElem = (GraphicElement *)modelVal.graphicElements.GetCurrent();

				graphElem->Print(&sdlHelper);

			}while(modelVal.graphicElements.MoveNext());
		}

		sdlHelper.Refresh();

		sdlHelper.WaitForKey();

		sdlHelper.Quit();

		#else

		if(argc > 1)
		{
			char * fileName = argv[1];

			Tag * rootNode = parser.ParseFile(fileName);

			rootNode->Print();

			ModelValidator modelVal;
			
			modelVal.ParseAndValidate(rootNode);

			SDLHelper sdlHelper;

			sdlHelper.Initialize(800, 600, 32);

			if(!modelVal.graphicElements.IsEmpty())
			{
				modelVal.graphicElements.MoveFirst();

				do
				{
					GraphicElement * graphElem = (GraphicElement *)modelVal.graphicElements.GetCurrent();

					graphElem->Print(&sdlHelper);

				}while(modelVal.graphicElements.MoveNext());
			}

			sdlHelper.Refresh();

			sdlHelper.WaitForKey();

			sdlHelper.Quit();
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