#ifndef BATTLECITYTYPES_INCLUDED
#define BATTLECITYTYPES_INCLUDED

#include "list"
#include "vector"

using namespace std;

#define TEXTURE_NAME_MAX_LENGTH 50

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
	UP, DOWN, LEFT, RIGHT, NONE
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

        static bool Intersects ( Rect rect, Point p );

		virtual Rect GetRect();
};

class BattleCityTank : public BattleCityCollitionObject
{
	public:
		BattleCityTank(int size)
		{
			this->Width = size;
			this->Height = size;
            this->Direction = NONE;
		};

		double			Speed;
		Direction		Direction;
		unsigned int	Life;
        char        	TextureName[TEXTURE_NAME_MAX_LENGTH];
};


class BattleCityBomb : public BattleCityCollitionObject
{
    private:
        int explodingWidth;
        int explodingHeight;	
    public:
		BattleCityBomb(int size, int explodingSize)
		{
			this->Width = size;
			this->Height = size;

            this->explodingWidth = explodingSize;
            this->explodingHeight = explodingSize;
		};

        Rect GetExplodedRect();

		unsigned int	Tank;
		int				TimeToDie;
};

class BattleCityBullet : public BattleCityCollitionObject
{
    public:
		BattleCityBullet(int size)
		{
			this->Width = size;
			this->Height = size;
		};

	    unsigned int	Tank;
	    int				DistanceToDie;
	    Direction		Direction;
};

class BattleCityScenario
{
    private:
        Rect quadrants[4];
        int width, height;

    public:
        BattleCityScenario(int width, int height);

        Rect GetQuadrant(DoublePoint point);
        Rect GetQuadrant(Point point);
};

class BattleCityTexture
{
    private:
        char name[TEXTURE_NAME_MAX_LENGTH];
        char * path;
    public:
        BattleCityTexture(char * name, char * path);

        char * GetName() { return this->name; };
        char * GetPath() { return this->path; };
};

#endif
