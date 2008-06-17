#include "BattleCityWall.h"

#include "math.h"
#include "exception"


BattleCityWall::BattleCityWall(Rect rect,BattleCityWallTypes type) : type(type)
{
	this->Pos.X = rect.X;
	this->Pos.Y = rect.Y;
	this->Width = rect.Width;
	this->Height = rect.Height;

	if ( type == WOOD )
		life = 2;
	else if ( type == ROCK )
		life = 4;
	else if ( type == IRON )
		life = 1;
	else 
		throw exception ( "BattleCityWall::BattleCityWall Invalid wall type" );
}

#define DISTANCE(x1,y1,x2,y2) sqrt((double) ((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2)))

unsigned int BattleCityWall::Distance(DoublePoint p)
{
	if ( p.X >= this->Pos.X && p.X < (this->Pos.X + this->Width) )
	{
		if ( p.Y < this->Pos.Y )
			return this->Pos.Y - p.Y;
		else if ( p.Y >= (this->Pos.Y + this->Height) )
			return p.Y - (this->Pos.Y + this->Height - 1);
		else 
			return 0;
	}
	else if (p.Y >= this->Pos.Y && p.Y < (this->Pos.Y + this->Height) )
	{
		if ( p.X < this->Pos.X )
			return this->Pos.X - p.X;
		else if ( p.X >= (this->Pos.X + this->Width) )
			return p.X - (this->Pos.X + this->Width - 1);
		else
			return 0;
	}
	else
	{
		unsigned int v1, v2, v3, v4;
		v1 = (unsigned int) DISTANCE(p.X,p.Y,this->Pos.X,this->Pos.Y);
		v2 = (unsigned int) DISTANCE(p.X,p.Y,this->Pos.X+this->Width,this->Pos.Y);
		v3 = (unsigned int) DISTANCE(p.X,p.Y,this->Pos.X,this->Pos.Y+this->Height);
		v4 = (unsigned int) DISTANCE(p.X,p.Y,this->Pos.X+this->Width,this->Pos.Y+this->Height);
		
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

int BattleCityWall::GetLife()
{
	return life;
}


BattleCityWallTypes BattleCityWall::GetType()
{
	return type;
}