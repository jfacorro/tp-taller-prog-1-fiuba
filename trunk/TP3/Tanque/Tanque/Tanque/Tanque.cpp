// Tanque.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"

#include "exception"
#include "iostream"

#include "BattleCityServer.h"

/*
#include "BattleCityEngine.h"


vector<BattleCityWall> walls;
*/

BattleCityParameters ParseParameters()
{
	BattleCityParameters p;

	p.ArenaHeight = 24;
	p.ArenaWidth = 80;
	p.TankRadius = 0;

	BattleCityTank t;
	t.Pos.X = 20;
	t.Pos.Y = 10;
	t.Speed = 2.0;
	t.Life = 10;
	t.Direction = RIGHT;
	p.Tanks.push_back(t);
	t.Pos.X = 60;
	t.Pos.Y = 10;
	t.Speed = 2.0;
	t.Life = 10;
	t.Direction = LEFT;
	p.Tanks.push_back(t);

	p.MaxBullets = 5;
	p.BulletSpeed = 25;
	p.BulletScope = 25;

	p.MaxBombs = 3;
	p.BombDelay = 5000;
	p.BombBlastDelay = 1000;
	p.BombBlastRadius = 3;

	Rect rect;
	rect.X = 2;
	rect.Y = 2;
	rect.Width = 4;
	rect.Height = 4;

	BattleCityWall w ( rect , ROCK );	
	p.Walls.push_back(w);

	rect.X = 2;
	rect.Y = 12;
	rect.Width = 4;
	rect.Height = 4;

	BattleCityWall w2 ( rect , IRON );	
	p.Walls.push_back(w2);

	for ( int i = 10 ; i < 20 ; i++ )
	{
		Rect rect;
		rect.X = 3 * i + 1;
		rect.Y = i + 4;
		rect.Width = 1;
		rect.Height = 20 - i;

		BattleCityWall wall ( rect , WOOD );
		p.Walls.push_back(wall);
	}
		

	return p;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;

	try
	{
		BattleCityParameters p = ParseParameters();
		BattleCityServer srv ( 2488 , p );
		srv.Start();
	}
	catch ( exception ex )
	{
		cerr << ex.what();
	}

	return retval;
}
