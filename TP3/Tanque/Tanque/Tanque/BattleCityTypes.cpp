#ifndef BattleCityTypes_cpp
#define BattleCityTypes_cpp

#include "BattleCityTypes.h"

Rect BattleCityCollitionObject::GetRect()
{
	Rect rect;
	rect.Height = this->Height;
	rect.Width = this->Width;
	rect.X = this->Pos.X;
	rect.Y = this->Pos.Y;

	return rect;
};

#define POINT_INSIDE_RECT(x,y,rect) (rect.X <= x && (rect.X + rect.Width) > x && rect.Y <= y && (rect.Y + rect.Height) > y)

bool BattleCityCollitionObject::Intersects(Point p)
{
	return POINT_INSIDE_RECT(p.X,p.Y,this->GetRect());
}

bool BattleCityCollitionObject::Intersects(Rect rval)
{
	Rect rect = this->GetRect();
	return !( (rval.X >= (rect.X + rect.Width)) ||
		      ((rval.X + rval.Width) <= rect.X) ||
			  (rval.Y >= (rect.Y + rect.Height)) ||
			  ((rval.Y + rval.Height) <= rect.Y) );
}

bool BattleCityCollitionObject::Intersects(BattleCityCollitionObject &collObject)
{
	return Intersects ( collObject.GetRect());
}


#endif