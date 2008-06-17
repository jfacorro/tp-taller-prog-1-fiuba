#ifndef BATTLECITYWALL_INCLUDED
#define BATTLECITYWALL_INCLUDED

#include "BattleCityTypes.h"


enum BattleCityWallTypes
{
	WOOD,
	ROCK,
	IRON
};

class BattleCityWall : public BattleCityCollitionObject
{
public:
    BattleCityWall () {};
	BattleCityWall ( Rect rect , BattleCityWallTypes type );

	unsigned int Distance ( DoublePoint p );

	int Shoot ();
	int	Blast ();

	int	GetLife();
	BattleCityWallTypes GetType();

private:
	BattleCityWallTypes type;
	int					life;
};

#endif
