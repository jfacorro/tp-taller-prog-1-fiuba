#ifndef BattleCityTypes_cpp
#define BattleCityTypes_cpp

#include "BattleCityTypes.h"
#include "StringHelper.h"

/**********************************************************************************************************/
// BattleCityCollitionObject
/**********************************************************************************************************/
Rect BattleCityCollitionObject::GetRect()
{
	Rect rect;
	rect.Height = this->Height;
	rect.Width = this->Width;
	rect.X = this->Pos.X - this->Width / 2;
	rect.Y = this->Pos.Y - this->Height / 2;;

	return rect;
};

#define POINT_INSIDE_RECT(x,y,rect) (rect.X <= x && (rect.X + rect.Width) > x && rect.Y <= y && (rect.Y + rect.Height) > y)

bool BattleCityCollitionObject::Intersects ( Rect rect, Point p )
{
    return POINT_INSIDE_RECT(p.X, p.Y, rect);
}

bool BattleCityCollitionObject::Intersects(Point p)
{
    return BattleCityCollitionObject::Intersects(this->GetRect(), p);
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
	return Intersects(collObject.GetRect());
}


/**********************************************************************************************************/
// BattleCityScenario
/**********************************************************************************************************/
BattleCityScenario::BattleCityScenario(int width, int height) : width(width), height(height)
{
    int quadrantHeight = height / 2;
    int quadrantWidth = width / 2;
    
    this->quadrants[0].X = 0;
    this->quadrants[0].Y = 0;
    this->quadrants[0].Width = quadrantWidth;
    this->quadrants[0].Height = quadrantHeight;

    this->quadrants[1].X = quadrantWidth;
    this->quadrants[1].Y = 0;
    this->quadrants[1].Width = quadrantWidth;
    this->quadrants[1].Height = quadrantHeight;

    this->quadrants[2].X = 0;
    this->quadrants[2].Y = quadrantHeight;
    this->quadrants[2].Width = quadrantWidth;
    this->quadrants[2].Height = quadrantHeight;

    this->quadrants[3].X = quadrantWidth;
    this->quadrants[3].Y = quadrantHeight;
    this->quadrants[3].Width = quadrantWidth;
    this->quadrants[3].Height = quadrantHeight;
}

Rect BattleCityScenario::GetQuadrant(DoublePoint point)
{
    Point p;
    p.X = (int)point.X;
    p.Y = (int)point.Y;

    return this->GetQuadrant(p);
}

Rect BattleCityScenario::GetQuadrant(Point point)
{
    Rect quadrant;

    for(int i = 0; i < 4; i++)
    {
        if(BattleCityCollitionObject::Intersects(this->quadrants[i], point))
        {
            quadrant = this->quadrants[i];
        }
    }

    return quadrant;
}



/**********************************************************************************************************/
// BattleCityTexture
/**********************************************************************************************************/
BattleCityTexture::BattleCityTexture(char * name, char * path)
{
    this->path = new char[strlen(path)];

    StringHelper::Copy(name, this->name, strlen(name));
    StringHelper::Copy(path, this->path, strlen(path));
}

#endif