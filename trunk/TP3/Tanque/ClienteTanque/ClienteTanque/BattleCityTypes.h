#ifndef BATTLECITYTYPES_INCLUDED
#define BATTLECITYTYPES_INCLUDED


#include "list"
#include "vector"

using namespace std;


typedef struct _Point
{
	int X;
	int Y;
}
	Point;


typedef struct _DoublePoint
{
	double X;
	double Y;
}
	DoublePoint;


typedef enum _Direction
{
	UP, DOWN, LEFT, RIGHT
}
	Direction;	


typedef struct _BattleCityTank
{
	DoublePoint		Pos;
	double			Speed;
	Direction		Direction;
	unsigned int	Life;
}	
	BattleCityTank;


typedef struct _BattleCityBomb
{
	unsigned int	Tank;
	int				TimeToDie;
	Point			Pos;
}	
	BattleCityBomb;


typedef struct _BattleCityBullet
{
	unsigned int	Tank;
	int				DistanceToDie;
	DoublePoint		Pos;
	Direction		Direction;
}
	BattleCityBullet;


typedef struct _Rect
{
	int	X;
	int Y;
	int Height;
	int Width;
}
	Rect;

#endif
