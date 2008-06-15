#include "BattleCityWall.h"

#include "math.h"
#include "exception"


BattleCityWall::BattleCityWall(Rect rect,BattleCityWallTypes type) : rect(rect), type(type)
{
	if ( type == WOOD )
		life = 2;
	else if ( type == ROCK )
		life = 4;
	else if ( type == IRON )
		life = 1;
	else 
		throw exception ( "BattleCityWall::BattleCityWall Invalid wall type" );
}


#define POINT_INSIDE_RECT(x,y,rect) (rect.X <= x && (rect.X + rect.Width) > x && rect.Y <= y && (rect.Y + rect.Height) > y)


bool BattleCityWall::Intersects(Point p)
{
	return POINT_INSIDE_RECT(p.X,p.Y,rect);
}

bool BattleCityWall::Intersects(Rect rval)
{
	return !( (rval.X >= (rect.X + rect.Width)) ||
		      ((rval.X + rval.Width) <= rect.X) ||
			  (rval.Y >= (rect.Y + rect.Height)) ||
			  ((rval.Y + rval.Height) <= rect.Y) );
}


bool BattleCityWall::Intersects(BattleCityWall &wall)
{
	return Intersects ( wall.rect );
}


#define DISTANCE(x1,y1,x2,y2) sqrt((double) ((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2)))

unsigned int BattleCityWall::Distance(Point p)
{
	if ( p.X >= rect.X && p.X < (rect.X + rect.Width) )
	{
		if ( p.Y < rect.Y )
			return rect.Y - p.Y;
		else if ( p.Y >= (rect.Y + rect.Height) )
			return p.Y - (rect.Y + rect.Height - 1);
		else 
			return 0;
	}
	else if (p.Y >= rect.Y && p.Y < (rect.Y + rect.Height) )
	{
		if ( p.X < rect.X )
			return rect.X - p.X;
		else if ( p.X >= (rect.X + rect.Width) )
			return p.X - (rect.X + rect.Width - 1);
		else
			return 0;
	}
	else
	{
		unsigned int v1, v2, v3, v4;
		v1 = (unsigned int) DISTANCE(p.X,p.Y,rect.X,rect.Y);
		v2 = (unsigned int) DISTANCE(p.X,p.Y,rect.X+rect.Width,rect.Y);
		v3 = (unsigned int) DISTANCE(p.X,p.Y,rect.X,rect.Y+rect.Height);
		v4 = (unsigned int) DISTANCE(p.X,p.Y,rect.X+rect.Width,rect.Y+rect.Height);
		
		unsigned int min = v1;
		if ( v2 < min ) min = v2;
		if ( v3 < min ) min = v3;
		if ( v4 < min ) min = v4;

		return min;
	}	
}


int BattleCityWall::Shoot()
{
	if ( type != IRON )
		life--;

	return life;
}


int BattleCityWall::Blast()
{
	if ( type != IRON )
		life -= 2;

	return life;
}


Rect BattleCityWall::GetRect()
{
	return rect;
}


int BattleCityWall::GetLife()
{
	return life;
}


BattleCityWallTypes BattleCityWall::GetType()
{
	return type;
}