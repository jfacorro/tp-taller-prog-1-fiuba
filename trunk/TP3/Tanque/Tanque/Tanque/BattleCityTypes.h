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

typedef struct _Rect
{
	int	X;
	int Y;
	int Height;
	int Width;
}
	Rect;

class BattleCityCollitionObject
{
	protected:
		int				Width;
		int				Height;
	public:
		BattleCityCollitionObject()
		{
			this->Width = 1;
			this->Height = 1;
		};

		DoublePoint		Pos;

		bool Intersects ( Point p );
		bool Intersects ( Rect rect );
		bool Intersects ( BattleCityCollitionObject& collObject );

		Rect GetRect();
};

class BattleCityTank : public BattleCityCollitionObject
{
	public:
		BattleCityTank(int size)
		{
			this->Width = size;
			this->Height = size;
		};

		double			Speed;
		Direction		Direction;
		unsigned int	Life;
};


class BattleCityBomb: public BattleCityCollitionObject
{
	public:
		unsigned int	Tank;
		int				TimeToDie;
};


typedef struct _BattleCityBullet
{
	unsigned int	Tank;
	int				DistanceToDie;
	DoublePoint		Pos;
	Direction		Direction;
}
	BattleCityBullet;

#endif
