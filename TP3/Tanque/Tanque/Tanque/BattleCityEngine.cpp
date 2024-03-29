#ifndef BattleCityEngine_cpp
#define BattleCityEngine_cpp

#include "BattleCityEngine.h"
#include "StringHelper.h"

BattleCityClientParameters GetBattleCityClientParameters(BattleCityParameters params)
{
    BattleCityClientParameters clientParameters;
    clientParameters.ArenaHeight = params.ArenaHeight;
    clientParameters.ArenaWidth = params.ArenaWidth;

    clientParameters.BombBlastDelay = params.BombBlastDelay;
    clientParameters.BombBlastRadius = params.BombBlastRadius;
    clientParameters.BombDelay = params.BombDelay;
    clientParameters.BombRadius = params.BombRadius;

    clientParameters.PixelsPerUM = params.PixelsPerUM;
    StringHelper::Copy(params.BackGroundTextureId, clientParameters.BackGroundTextureId, strlen(params.BackGroundTextureId));
    StringHelper::Copy(params.ExplosionTextureId, clientParameters.ExplosionTextureId, strlen(params.ExplosionTextureId));

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
}

BattleCityEngine::~BattleCityEngine()
{
}

bool BattleCityEngine::GetDirty()
{
	return dirty;
}

bool BattleCityEngine::GetFinished()
{
	return finished;
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
	for ( int i = 0 ; i < tanks.size() ; i++ )
		parameters.Tanks[i].Points = tanks[i].Points;

	tanks.assign(parameters.Tanks.begin(),parameters.Tanks.end());
	walls.assign(parameters.Walls.begin(),parameters.Walls.end());
	dirty = true;
	finished = false;

	#ifdef BATTLECITYENGINE_WALL_TEST
	for ( unsigned int j = 0 ; j < parameters.Walls.size() ; j++ )
		for ( unsigned int k = j + 1 ; k < parameters.Walls.size() ; k++ )
			if ( parameters.Walls[j].Intersects(parameters.Walls[k]) )
				throw exception ( "BattleCityEngine::BattleCityEngine Intersecting wall found" );
	#endif

	nextTick = GetTickCount();
	UpdateNextTick();
}

void BattleCityEngine::UpdateBombs()
{
	vector<BattleCityBomb> toDie;
	for ( int i = 0 ; i < bombs.size() ; i++ )
	{        
		bombs[i].TimeToDie -= (nextTick - lastTick);
		if ( bombs[i].TimeToDie < - (int) parameters.BombBlastDelay )
		{
			toDie.push_back(bombs[i]);
			dirty = true;
		}
		else if ( bombs[i].TimeToDie <= 0 && (bombs[i].TimeToDie + (int) nextTick - (int) lastTick) > 0)
		{
			dirty = true;

			unsigned int j = 0;

			for ( j = 0 ; j < tanks.size() ; j++ )
			{
                if (tanks[j].Intersects(bombs[i].GetExplodedRect()) && tanks[j].Life > 0)
                {
					HitTank(j, BATTLE_CITY_BOMB_HIT_ENERGY);

                    BattleCityTank hittedTank = tanks[j];

                    if((hittedTank.Life <= 0 || hittedTank.Life > parameters.Tanks[j].Life) && (j != bombs[i].Tank))
                    {
                        tanks[bombs[i].Tank].Points += BATTLE_CITY_POINTS_TANK;
                    }
                }
			}

			for ( j = 0 ; j < walls.size() ; j++ )
            {
				if ( walls[j].Intersects(bombs[i].GetExplodedRect()))
                {
					if ( walls[j].Blast() <= 0 )
					{
                        switch(walls[j].GetType())
                        {
                            case WOOD:
                                tanks[bombs[i].Tank].Points += BATTLE_CITY_POINTS_WOOD;
                                break;
                            case ROCK:
                                tanks[bombs[i].Tank].Points += BATTLE_CITY_POINTS_ROCK;
                                break;
                        }
                        walls.erase(walls.begin()+j);
						j--;
					}
                }
            }
		}
	}

	if ( !toDie.empty() )
    {
        for ( int y = 0 ; y < toDie.size() ; y++ )
		{
            for ( int iterToDie = 0 ; iterToDie < bombs.size() ; iterToDie++ )
            {
				if ( bombs[y].Pos.X == toDie[y].Pos.X && bombs[iterToDie].Pos.Y == toDie[y].Pos.Y )
				{
                    bombs.erase(bombs.begin() + iterToDie);
					break;
				}
            }
		}
    }
}

void BattleCityEngine::UpdateNextTick()
{
	lastTick = nextTick;

	/*
	double max = parameters.BulletSpeed;
	for ( unsigned int i = 0 ; i < parameters.Tanks.size() ; i++ )
		if ( parameters.Tanks[i].Speed > max )
			max = parameters.Tanks[i].Speed;

	nextTick += (int) (1000.0 / max);
	*/

	nextTick += (int) (1000.0 / FRAMES_PER_SECOND);
}

void BattleCityEngine::HitTank(unsigned int tank, int decrementedEnergy)
{
	if ( tanks[tank].Life > 0 )
    {
		tanks[tank].Life -= decrementedEnergy;
    }
	else
	{
        
	}

	if ( tanks[tank].Life > parameters.Tanks[tank].Life )
		tanks[tank].Life = 0;

	if ( tanks[tank].Life <= 0 || tanks[tank].Life > parameters.Tanks[tank].Life )
		finished = true;
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

bool BattleCityEngine::UpdateBulletPos(unsigned int bullet,double currentX,double currentY,double nextX,double nextY,bool check)
{
	bool hit = false;
    BattleCityBullet currentBullet = bullets[bullet];
    
    Rect currentRect = currentBullet.GetRect();
    Rect nextRect = currentBullet.GetRect();
    nextRect.X = nextX;
    nextRect.Y = nextY;

	if ( currentX < nextX )
	{
		nextRect.X = currentX;
		nextRect.Width += (nextX - currentX);
	}
	else
	{
		nextRect.X = nextX;
		nextRect.Width += (currentX - nextX);
	}

	if ( currentY < nextY )
	{
		nextRect.Y = currentY;
		nextRect.Height += (nextY - currentY);
	}
	else
	{
		nextRect.Y = nextY;
		nextRect.Height += (currentY - nextY);
	}

	for ( unsigned int i = 0 ; i < tanks.size() && !hit ; i++ )
    {
        if (tanks[i].Intersects(currentRect) && tanks[i].Intersects(nextRect) && tanks[i].Life > 0)
		{
			if ( !check && bullets[bullet].Tank == i )
			{

			}
			else
			{
			HitTank(i, BATTLE_CITY_BULLET_HIT_ENERGY);

            BattleCityTank hittedTank = tanks[i];

            if((hittedTank.Life <= 0) && (i != bullets[bullet].Tank))
            {
                tanks[bullets[bullet].Tank].Points += BATTLE_CITY_POINTS_TANK;
            }

			hit = true;
		}
    }
    }



	/*
	for ( unsigned int j = 0 ; j < walls.size() && !hit ; j++ )
	{
		if ( walls[j].GetType() == IRON )
		{
			switch(bullets[bullet].Direction)
			{
				case LEFT:

					bullets[bullet].Direction = RIGHT;
					break;
				case RIGHT:
    				bullets[bullet].Direction = LEFT;
					break;
				case UP:
    				bullets[bullet].Direction = DOWN;
					break;
				case DOWN:
					bullets[bullet].Direction = UP;
				break;
			}
		}
		else 
		{
			if ( walls[j].Intersects(nextRect))
			{
				hit = true;
				if ( walls[j].Shoot() <= 0 )
				{
					switch(walls[j].GetType())
					{
						case WOOD:
							tanks[bullets[bullet].Tank].Points += BATTLE_CITY_POINTS_WOOD;
							break;
						case ROCK:
							tanks[bullets[bullet].Tank].Points += BATTLE_CITY_POINTS_ROCK;
							break;
					}

					walls.erase(walls.begin() + j);
					j--;
				}
			}
		}
	}
	*/

	if ( !hit )
	{
		for ( unsigned int j = 0 ; j < walls.size() && !hit ; j++ )
		{
            if ( walls[j].Intersects(nextRect))
			{
				if ( walls[j].GetType() == IRON )
				{
					switch(bullets[bullet].Direction)
                    {
                        case LEFT:
						    bullets[bullet].Direction = RIGHT;
                            break;
                        case RIGHT:
    						bullets[bullet].Direction = LEFT;
                            break;
                        case UP:
    						bullets[bullet].Direction = DOWN;
                            break;
                        case DOWN:
    						bullets[bullet].Direction = UP;
                            break;
                    }

						nextX = currentX;
						nextY = currentY;
				}
				else 
				{
					hit = true;
					if ( walls[j].Shoot() <= 0 )
					{
                        switch(walls[j].GetType())
                        {
                            case WOOD:
                                tanks[bullets[bullet].Tank].Points += BATTLE_CITY_POINTS_WOOD;
                                break;
                            case ROCK:
                                tanks[bullets[bullet].Tank].Points += BATTLE_CITY_POINTS_ROCK;
                                break;
                        }

						walls.erase(walls.begin() + j);
						j--;
					}
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
        {
            bullets[bullet].DistanceToDie -= abs((nextX - currentX) + (nextY - currentY));

			if ( bullets[bullet].DistanceToDie <= 0 ) 
				hit = true;
        }
	}

	return hit;
}

void BattleCityEngine::Tick()
{
	unsigned int i;
	dirty = false;

	double dt = (double) (nextTick - lastTick) / 1000.0;

	/*
	double dt = (double) (nextTick - lastTick) / 1000.0;
	for ( i = 0 ; i < tanks.size() ; i++ )
	{
        if(tanks[i].Life > 0)
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
	}
	*/

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

	for ( i = 0 ; i < tanks.size() ; i++ )
	{
        if(tanks[i].Life > 0)
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
	}

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

    for (int iter = 0; iter < bombs.size() ; iter ++)
	{
		if ( bombs[iter].Tank == tank )
			bombCount++;
		if ( bombs[iter].Pos.X == (int) tanks[tank].Pos.X && bombs[iter].Pos.Y == (int) tanks[tank].Pos.Y )
			hasBomb = true;
	}

	if ( bombCount >= parameters.MaxBombs || hasBomb )
		return false;

    BattleCityBomb b(this->parameters.BombRadius * 2, this->parameters.BombBlastRadius * 2);
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

    BattleCityBullet b(this->parameters.BulletRadius * 2);
	b.Tank = tank;
	b.Pos.X = (int) tanks[tank].Pos.X;
	b.Pos.Y = (int) tanks[tank].Pos.Y;
	b.DistanceToDie = parameters.BulletScope;
	b.Direction = tanks[tank].Direction;

    bullets.push_back(b);

	double nextX = b.Pos.X;
	double nextY = b.Pos.Y;
    switch(b.Direction)
    {
        case LEFT:
            nextX -= this->parameters.TankRadius + b.GetRect().Width / 2 + 1;
            break;
        case RIGHT:
            nextX += this->parameters.TankRadius + b.GetRect().Width / 2 + 1;
            break;
        case UP:
            nextY -= this->parameters.TankRadius + b.GetRect().Height / 2 + 1;
            break;
        case DOWN:
            nextY += this->parameters.TankRadius + b.GetRect().Height / 2 + 1;
            break;
    }

	if ( UpdateBulletPos ( bullets.size() - 1 , b.Pos.X , b.Pos.Y , nextX , nextY , false ) )
		bullets.pop_back();

    //bullets.push_back(b);

    return true;
}

#endif