#ifndef BATTLECITYWALL_INCLUDED
#define BATTLECITYWALL_INCLUDED

#include "BattleCityTypes.h"


enum BattleCityWallTypes
{
	WOOD = 0,
	ROCK = 1,
	IRON = 2
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
    Rect GetRect();

private:
	BattleCityWallTypes type;
	int					life;
};

#endif
