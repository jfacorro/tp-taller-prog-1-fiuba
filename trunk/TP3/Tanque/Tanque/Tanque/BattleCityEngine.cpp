#include "BattleCityEngine.h"

BattleCityClientParameters GetBattleCityClientParameters(BattleCityParameters params)
{
    BattleCityClientParameters clientParameters;
    clientParameters.ArenaHeight = params.ArenaHeight;
    clientParameters.ArenaWidth = params.ArenaWidth;

    clientParameters.BombBlastDelay = params.BombBlastDelay;
    clientParameters.BombBlastRadius = params.BombBlastRadius;
    clientParameters.BombDelay = params.BombDelay;
    clientParameters.BombRadius = params.BombRadius;

    clientParameters.BulletRadius = params.BulletRadius;
    clientParameters.BulletScope = params.BulletScope;
    clientParameters.BulletSpeed = params.BulletSpeed;

    clientParameters.MaxBombs = params.MaxBombs;
    clientParameters.MaxBullets = params.MaxBullets;
    clientParameters.TankRadius = params.TankRadius;

    return clientParameters;
}

BattleCityEngine::BattleCityEngine(BattleCityParameters parameters) : parameters(parameters)
{
	if ( parameters.ArenaWidth == 0 || parameters.ArenaHeight == 0 )
		throw exception("BattleCityEngine::BattleCityEngine Invalid parameters");

	tanks.assign(parameters.Tanks.begin(),parameters.Tanks.end());
	walls.assign(parameters.Walls.begin(),parameters.Walls.end());
	dirty = true;

	#ifdef BATTLECITYENGINE_WALL_TEST
	for ( unsigned int j = 0 ; j < parameters.Walls.size() ; j++ )
		for ( unsigned int k = j + 1 ; k < parameters.Walls.size() ; k++ )
			if ( parameters.Walls[j].Intersects(parameters.Walls[k]) )
				throw exception ( "BattleCityEngine::BattleCityEngine Intersecting wall found" );
	#endif
}

BattleCityEngine::~BattleCityEngine()
{
}

bool BattleCityEngine::GetDirty()
{
	return dirty;
}

BattleCityState BattleCityEngine::GetState()
{
	BattleCityState retval;
	retval.Tanks.assign(tanks.begin(),tanks.end());
	retval.Bombs.assign(bombs.begin(),bombs.end());
	retval.Bullets.assign(bullets.begin(),bullets.end());
	retval.Walls.assign(walls.begin(),walls.end());
	return retval;
}

void BattleCityEngine::Start()
{
	nextTick = GetTickCount();
	UpdateNextTick();
}

void BattleCityEngine::UpdateBombs()
{
	list<BattleCityBomb> toDie;
	list<BattleCityBomb>::iterator iter;
	for ( iter = bombs.begin() ; iter != bombs.end() ; ++iter )
	{
		iter->TimeToDie -= (nextTick - lastTick);
		if ( iter->TimeToDie < - (int) parameters.BombBlastDelay )
		{
			toDie.push_back(*iter);
			dirty = true;
		}
		else if ( iter->TimeToDie <= 0 && (iter->TimeToDie + (int) nextTick - (int) lastTick) > 0)
		{
			dirty = true;
			int blastradius = parameters.BombBlastRadius * parameters.BombBlastRadius;

			for ( unsigned int i = 0 ; i < tanks.size() ; i++ )
			{
				int distance = (((int) tanks[i].Pos.X - iter->Pos.X) * ((int) tanks[i].Pos.X - iter->Pos.X) +
								((int) tanks[i].Pos.Y - iter->Pos.Y) * ((int) tanks[i].Pos.Y - iter->Pos.Y));
				if ( distance <= blastradius ) 
					HitTank(i);
			}

			for ( unsigned int j = 0 ; j < walls.size() ; j++ )
				if ( walls[j].Distance(iter->Pos) <= parameters.BombBlastRadius )
					if ( walls[j].Blast() <= 0 )
					{
						walls.erase(walls.begin()+j);
						j--;
					}
		}
	}

	if ( !toDie.empty() )
		for ( iter = toDie.begin() ; iter != toDie.end() ; ++iter )
		{
			list<BattleCityBomb>::iterator iterToDie;
			for ( iterToDie = bombs.begin() ; iterToDie != bombs.end() ; ++iterToDie )
				if ( iterToDie->Pos.X == iter->Pos.X && iterToDie->Pos.Y == iter->Pos.Y )
				{
					bombs.erase(iterToDie);
					break;
				}
		}
}

void BattleCityEngine::UpdateNextTick()
{
	lastTick = nextTick;

	double max = parameters.BulletSpeed;
	for ( unsigned int i = 0 ; i < parameters.Tanks.size() ; i++ )
		if ( parameters.Tanks[i].Speed > max )
			max = parameters.Tanks[i].Speed;

	nextTick += (int) (1000.0 / max);
}

void BattleCityEngine::HitTank(unsigned int tank)
{
	if ( tanks[tank].Life != 0 )
		tanks[tank].Life--;
	else
	{

	}
}

bool BattleCityEngine::FindTankCollition(unsigned int tank, int x, int y)
{
	BattleCityTank nextPosTank(2 * this->parameters.TankRadius);
	nextPosTank.Pos.X = x;
	nextPosTank.Pos.Y = y;

	for ( unsigned int i = 0 ; i < tanks.size() ; i++ )
		if ( i != tank )
			if ( nextPosTank.Intersects(tanks[i]) )
				return true;

	for ( unsigned int j = 0 ; j < walls.size() ; j++ )
		if ( walls[j].Intersects(nextPosTank) )
			return true;

    if
    (
        nextPosTank.GetRect().X < 0 || 
        nextPosTank.GetRect().Y < 0 || 
        nextPosTank.GetRect().X > this->parameters.ArenaWidth - nextPosTank.GetRect().Width || 
        nextPosTank.GetRect().Y > this->parameters.ArenaHeight - nextPosTank.GetRect().Height
    )
    {
        return true;
    }

	return false;
}

void BattleCityEngine::UpdateTankPos(unsigned int tank,double nextX,double nextY)
{
	if ( FindTankCollition(tank,(int)nextX,(int)nextY) )
		tanks[tank].Speed = 0;
	else
	{
		tanks[tank].Pos.X = nextX;
		tanks[tank].Pos.Y = nextY;
		
		dirty = true;		
	}
}

bool BattleCityEngine::UpdateBulletPos(unsigned int bullet,double currentX,double currentY,double nextX,double nextY)
{
	bool hit = false;
    Point nextPoint, currentPoint;
    nextPoint.X = nextX;
    nextPoint.Y = nextY;
    currentPoint.X = currentX;
    currentPoint.Y = currentY;
	for ( unsigned int i = 0 ; i < tanks.size() && !hit ; i++ )
        if 
        (
            // bullets[bullet].Tank != i && 
            tanks[i].Intersects(nextPoint) && 
            tanks[i].Intersects(currentPoint) 
        )
		{
			HitTank(i);
			hit = true;
		}

	if ( !hit )
	{
		Point p;
		p.X = (int) nextX;
		p.Y = (int) nextY;
		for ( unsigned int j = 0 ; j < walls.size() && !hit ; j++ )
			if ( walls[j].Intersects(p) )
			{
				if ( walls[j].GetType() == IRON )
				{
					if ( bullets[bullet].Direction == LEFT ) 
						bullets[bullet].Direction = RIGHT;
					else if ( bullets[bullet].Direction == RIGHT )
						bullets[bullet].Direction = LEFT;
					else if ( bullets[bullet].Direction == UP )
						bullets[bullet].Direction = DOWN;
					else if ( bullets[bullet].Direction == DOWN )
						bullets[bullet].Direction = UP;
				}
				else 
				{
					hit = true;
					if ( walls[j].Shoot() <= 0 )
					{
						walls.erase(walls.begin() + j);
						j--;
					}
				}
			}
	}

	if ( !hit )
	{
		bullets[bullet].Pos.X = nextX;
		bullets[bullet].Pos.Y = nextY;

		if ( bullets[bullet].Pos.X < 0 || bullets[bullet].Pos.Y < 0 ||
			 bullets[bullet].Pos.X >= parameters.ArenaWidth ||
			 bullets[bullet].Pos.Y >= parameters.ArenaHeight 
			) 
				hit = true;

		if ( (int) currentX != (int) nextX || (int) currentY != (int) nextY )
			if ( bullets[bullet].DistanceToDie-- == 0 ) 
				hit = true;
	}

	return hit;
}

void BattleCityEngine::Tick()
{
	unsigned int i;
	dirty = false;

	double dt = (double) (nextTick - lastTick) / 1000.0;
	for ( i = 0 ; i < tanks.size() ; i++ )
	{
		if ( tanks[i].Direction == LEFT ) 
			UpdateTankPos ( i , tanks[i].Pos.X - dt * tanks[i].Speed , tanks[i].Pos.Y );
		else if ( tanks[i].Direction == RIGHT ) 
			UpdateTankPos ( i , tanks[i].Pos.X + dt * tanks[i].Speed , tanks[i].Pos.Y );
		else if ( tanks[i].Direction == UP ) 
			UpdateTankPos ( i , tanks[i].Pos.X , tanks[i].Pos.Y - dt * tanks[i].Speed );
		else if ( tanks[i].Direction == DOWN )
			UpdateTankPos ( i , tanks[i].Pos.X , tanks[i].Pos.Y + dt * tanks[i].Speed );
	}

	list<unsigned long> bulletsToDie;
	for ( i = 0 ; i < bullets.size() ; i++ )
	{
		bool toDie = false;
		if ( bullets[i].Direction == LEFT ) 
			toDie = UpdateBulletPos ( i , bullets[i].Pos.X , bullets[i].Pos.Y , bullets[i].Pos.X - dt * parameters.BulletSpeed , bullets[i].Pos.Y );
		else if ( bullets[i].Direction == RIGHT ) 
			toDie = UpdateBulletPos ( i , bullets[i].Pos.X , bullets[i].Pos.Y , bullets[i].Pos.X + dt * parameters.BulletSpeed , bullets[i].Pos.Y );
		else if ( bullets[i].Direction == UP ) 
			toDie = UpdateBulletPos ( i , bullets[i].Pos.X , bullets[i].Pos.Y , bullets[i].Pos.X , bullets[i].Pos.Y - dt * parameters.BulletSpeed );
		else if ( bullets[i].Direction == DOWN )
			toDie = UpdateBulletPos ( i , bullets[i].Pos.X , bullets[i].Pos.Y , bullets[i].Pos.X , bullets[i].Pos.Y + dt * parameters.BulletSpeed );

		if ( toDie )
			bulletsToDie.push_back(i);
	}

	unsigned int bulletsToDieCount = 0;
	list<unsigned long>::iterator iter;
	for ( iter = bulletsToDie.begin() ; iter != bulletsToDie.end() ; ++iter, bulletsToDieCount++ )
		bullets.erase(bullets.begin() + *iter - bulletsToDieCount);

	UpdateNextTick();
	UpdateBombs();
}

int BattleCityEngine::GetNextTickInterval()
{
	DWORD tmp = GetTickCount();
	if ( tmp >= nextTick )
		return 0;
	else
		return nextTick - tmp;
}

void BattleCityEngine::TurnTank(unsigned int tank,Direction direction)
{
	if ( tank >= tanks.size() )
		throw exception ( "BattleCityEngine::TurnTank Invalid direction" );
	else
	{
		tanks[tank].Direction = direction;
		tanks[tank].Speed = parameters.Tanks[tank].Speed;
	}
}

bool BattleCityEngine::DropBomb(unsigned int tank)
{
	bool hasBomb = false;
	unsigned int bombCount = 0;
	list<BattleCityBomb>::iterator iter;
	for ( iter = bombs.begin() ; iter != bombs.end() ; ++iter )
	{
		if ( iter->Tank == tank )
			bombCount++;
		if ( iter->Pos.X == (int) tanks[tank].Pos.X && iter->Pos.Y == (int) tanks[tank].Pos.Y )
			hasBomb = true;
	}

	if ( bombCount >= parameters.MaxBombs || hasBomb )
		return false;

	BattleCityBomb b(this->parameters.BombRadius);
	b.Tank = tank;
	b.TimeToDie = parameters.BombDelay;
	DoublePoint p = tanks[tank].Pos;
	b.Pos.X = (int) p.X;
	b.Pos.Y = (int) p.Y;

	bombs.push_back(b);
	return true;
}

bool BattleCityEngine::ShootBullet(unsigned int tank)
{
	unsigned int bulletCount = 0;
	for ( unsigned int i = 0 ; i < bullets.size() ; i++ )
		if ( bullets[i].Tank == tank )
			bulletCount++;

	if ( bulletCount >= parameters.MaxBullets )
		return false;

    BattleCityBullet b(this->parameters.BulletRadius);
	b.Tank = tank;
	b.Pos.X = (int) tanks[tank].Pos.X;
	b.Pos.Y = (int) tanks[tank].Pos.Y;
	b.DistanceToDie = parameters.BulletScope;
	b.Direction = tanks[tank].Direction;

    switch(b.Direction)
    {
        case LEFT:
            b.Pos.X -= this->parameters.TankRadius + 1;
            break;
        case RIGHT:
            b.Pos.X += this->parameters.TankRadius + 1;
            break;
        case UP:
            b.Pos.Y -= this->parameters.TankRadius + 1;
            break;
        case DOWN:
            b.Pos.Y += this->parameters.TankRadius + 1;
            break;
    }

    bullets.push_back(b);

    return true;
}
