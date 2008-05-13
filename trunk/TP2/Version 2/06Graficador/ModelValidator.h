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

		bool BackColorAndTextureConflictExists(Tag * tag);

		void GetGraphicElement(Tag * tag);

		enum GraphicElementType {FIGURE, TEXTURE};
	public:
		void ParseAndValidate(Tag * rootNode);
		bool CheckDuplicatedId(ArrayList * elements, char * idStr, GraphicElementType type);

		ArrayList graphicElements;
		ArrayList textures;
		Configuration config;

		static Color ModelValidator::GetColorFromString(char * RGB);
};

#endif