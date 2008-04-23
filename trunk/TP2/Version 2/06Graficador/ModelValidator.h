#include "Tag.h"
#include "GraphicElement.h"
#include "ArrayList.h"

#ifndef ModelValidator_h

#define ModelValidator_h

class ModelValidator
{
	private:
		char * GetId(Tag * tag);
		Position GetPosition(Tag * tag, char * posTagName);
		Color GetColor(Tag * tag, char * colorAttName);
		SDL_Surface * GetTexture(Tag * tag);
		SDL_Surface * GetTextureById(char * textureId);

		void GetGraphicElement(Tag * tag);
	public:
		void ParseAndValidate(Tag * rootNode);
		bool CheckDuplicatedId(ArrayList * graphElementsArr, char * id);

		ArrayList graphicElements;
		ArrayList textures;
		Configuration config;

		static Color ModelValidator::GetColorFromString(char * RGB);
};

#endif