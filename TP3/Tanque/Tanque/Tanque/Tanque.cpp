// Tanque.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"

#include "exception"
#include "iostream"

#include "BattleCityServer.h"
#include "BattleCityCommunicationProtocol.h"
#include "StringHelper.h"

BattleCityParameters ParseParameters()
{
	BattleCityParameters p;

    double tankSpeed = 70;
	
    p.ArenaWidth = 1600;
	p.ArenaHeight = 1200;
	p.TankRadius = 20;
	p.BulletRadius = 3;
    p.BombRadius = 4;

	BattleCityTank t(p.TankRadius * 2);
	t.Pos.X = 150;
	t.Pos.Y = 50;
	t.Speed = tankSpeed;
	t.Life = 10;
	t.Direction = RIGHT;
    StringHelper::Copy("tank01", t.TextureName, strlen("tank01"));
	p.Tanks.push_back(t);
	t.Pos.X = 200;
	t.Pos.Y = 100;
	t.Speed = tankSpeed;
	t.Life = 10;
	t.Direction = LEFT;
    StringHelper::Copy("tank02", t.TextureName, strlen("tank02"));
	p.Tanks.push_back(t);

	p.MaxBullets = 5;
	p.BulletSpeed = tankSpeed * 3;
	p.BulletScope = p.BulletSpeed;

	p.MaxBombs = 3;
	p.BombDelay = 5000;
	p.BombBlastDelay = 1000;
	p.BombBlastRadius = 30;

	Rect rect;
	rect.X = 20;
	rect.Y = 20;
	rect.Width = 10;
	rect.Height = 30;

	BattleCityWall w ( rect , ROCK );
	p.Walls.push_back(w);

	rect.X = 40;
	rect.Y = 40;
	rect.Width = 50;
	rect.Height = 10;

	BattleCityWall w2 ( rect , IRON );
	p.Walls.push_back(w2);

    BattleCityTexture texture1("tank01", "tank01.bmp");
    p.Textures.push_back(texture1);

    BattleCityTexture texture2("tank02", "tank02.bmp");
    p.Textures.push_back(texture2);

    BattleCityTexture texture3("explosion01", "explosion.bmp");
    p.Textures.push_back(texture3);

	return p;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;

	try
	{
		BattleCityParameters p = ParseParameters();
		BattleCityServer srv ( BATTLE_CITY_SOCKET , p );
		srv.Start();
	}
	catch ( exception ex )
	{
		cerr << ex.what();
	}

	return retval;
}
