#include "Tag.h"
#include "GraphicElement.h"
#include "ArrayList.h"

#ifndef ModelValidator_h

#define ModelValidator_h

class ModelValidator
{
	private:
		char * ModelValidator::GetId(Tag * tag);
		Position GetPosition(Tag * tag, char * posTagName);
		Color GetColor(Tag * tag, char * colorAttName);
		SDL_Surface * GetTexture(Tag * tag);

		void GetGraphicElement(Tag * tag);
	public:
		void ParseAndValidate(Tag * rootNode);
		bool CheckDuplicatedId(ArrayList * graphElementsArr, char * id);

		ArrayList graphicElements;
		ArrayList textures;
		Configuration config;
};

#endif