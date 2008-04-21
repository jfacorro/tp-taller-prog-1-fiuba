#include "Tag.h"
#include "GraphicElement.h"
#include "ArrayList.h"

#ifndef ModelValidator_h

#define ModelValidator_h

class ModelValidator
{
	private:
		Position GetPosition(Tag * tag, char * posTagName);
		Color GetColor(Tag * tag, char * colorAttName);
		GraphicElement * GetGraphicElement(Tag * tag);
	public:
		void ParseAndValidate(Tag * rootNode);

		static Color GetDefaultColor()
		{
			Color color;
			color.R = 255;
			color.G = 255;
			color.B = 255;
			return color;
		};

		ArrayList graphicElements;
};

#endif