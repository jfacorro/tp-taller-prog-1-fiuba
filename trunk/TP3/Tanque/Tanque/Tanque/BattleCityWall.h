#ifndef BATTLECITYWALL_INCLUDED
#define BATTLECITYWALL_INCLUDED

#include "BattleCityTypes.h"


enum BattleCityWallTypes
{
	WOOD,
	ROCK,
	IRON
};

class BattleCityWall 
{
public:
	BattleCityWall ( Rect rect , BattleCityWallTypes type );

	bool Intersects ( Point p );
	bool Intersects ( Rect rect );
	bool Intersects ( BattleCityWall& wall );

	unsigned int Distance ( Point p );

	int  Shoot ();
	int	 Blast ();

	Rect	GetRect();
	int		GetLife();
	BattleCityWallTypes GetType();

private:
	Rect				rect;
	BattleCityWallTypes type;
	int					life;
};

#endif
