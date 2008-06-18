#ifndef BATTLECITYENGINE_INCLUDED
#define BATTLECITYENGINE_INCLUDED


#define BATTLECITYENGINE_WALL_TEST


#include "BattleCityTypes.h"
#include "BattleCityWall.h"

#include "windows.h"


typedef struct _BattleCityParameters
{
	unsigned int ArenaWidth;
	unsigned int ArenaHeight;
	unsigned int TankRadius;
	unsigned int BulletRadius;	
	
	vector<BattleCityTank> Tanks;
	vector<BattleCityWall> Walls;

	unsigned int	MaxBullets;
	double			BulletSpeed;
	unsigned int	BulletScope;

	unsigned int MaxBombs;
	unsigned int BombDelay;
	unsigned int BombBlastDelay;
	unsigned int BombBlastRadius;
} 
BattleCityParameters;

typedef struct _BattleCityClientParameters
{
	unsigned int ArenaWidth;
	unsigned int ArenaHeight;
	unsigned int TankRadius;
	unsigned int BulletRadius;	
	
	unsigned int	MaxBullets;
	double			BulletSpeed;
	unsigned int	BulletScope;

	unsigned int MaxBombs;
	unsigned int BombDelay;
	unsigned int BombBlastDelay;
	unsigned int BombBlastRadius;
} 
BattleCityClientParameters;

BattleCityClientParameters GetBattleCityClientParameters(BattleCityParameters params);

typedef struct _BattleCityState
{
	vector<BattleCityTank>		Tanks;
	vector<BattleCityBomb>		Bombs;
	vector<BattleCityBullet>	Bullets;
	vector<BattleCityWall>		Walls;
}
	BattleCityState;


class BattleCityEngine
{
public:
	BattleCityEngine(BattleCityParameters parameters);
	virtual ~BattleCityEngine();

	bool GetDirty();
	BattleCityState GetState();

	void Start();
	void Tick();
	int GetNextTickInterval();

	void TurnTank(unsigned int tank,Direction direction);
	bool DropBomb(unsigned int tank);
	bool ShootBullet(unsigned int tank);

private:
	void UpdateNextTick();
	void UpdateBombs();
	void UpdateTankPos(unsigned int tank,double nextX, double nextY);
	bool UpdateBulletPos(unsigned int bullet,double currentX,double currentY,double nextX,double nextY);

	void HitTank(unsigned int tank);
	bool FindTankCollition(unsigned int tank,int x, int y);

	BattleCityParameters parameters;

	bool dirty;
	DWORD lastTick;
	DWORD nextTick;

	vector<BattleCityTank>		tanks;
	list<BattleCityBomb>		bombs;	
	vector<BattleCityBullet>	bullets;
	vector<BattleCityWall>		walls;
};

#endif
