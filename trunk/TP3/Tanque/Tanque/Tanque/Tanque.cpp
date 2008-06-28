// Tanque.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"

#include "exception"
#include "iostream"

#include "BattleCityServer.h"
#include "BattleCityCommunicationProtocol.h"
#include "StringHelper.h"
#include "XMLParser.h"
#include "Exception.h"

BattleCityParameters ParseParameters(char * arenaFileName)
{
    BattleCityParameters p;

    double tankSpeed = 70;
	
    p.ArenaWidth = 1600;
	p.ArenaHeight = 1200;
	p.TankRadius = 20;
	p.BulletRadius = 3;
    p.BombRadius = 4;

    p.MaxBullets = 5;
	p.BulletSpeed = tankSpeed * 3;
	p.BulletScope = p.BulletSpeed;

	p.MaxBombs = 3;
	p.BombDelay = 5000;
	p.BombBlastDelay = 1000;
	p.BombBlastRadius = 30;

    /***************************************************/
    /// Parse XML file
    /***************************************************/
    XMLParser xmlParser;
    Tag * rootTag = xmlParser.ParseFile(arenaFileName);

    /// Get tanks textures
    Tag * texturesTag = rootTag->GetChildTag("Textures");

    /// Parse each texture tag
    if(!texturesTag->childTags.IsEmpty())
    {
        texturesTag->childTags.MoveFirst();

        do
        {
            Tag * textureTag = (Tag *)texturesTag->childTags.GetCurrent();

            char * textureId = textureTag->GetAttribute("id")->GetValue();
            char * texturePath = textureTag->GetAttribute("path")->GetValue();

            BattleCityTexture texture(textureId, texturePath);
            p.Textures.push_back(texture);

        }while(texturesTag->childTags.MoveNext());
    }    
    
    /// Get tanks tag
    Tag * tanksTag = rootTag->GetChildTag("Tanks");

    /// Parse each tank tag
    if(!tanksTag->childTags.IsEmpty())
    {
        tanksTag->childTags.MoveFirst();

        do
        {
            Tag * tankTag = (Tag *)tanksTag->childTags.GetCurrent();

            char * textureValue = tankTag->GetAttribute("texture")->GetValue();
            char * x = tankTag->GetAttribute("x")->GetValue();
            char * y = tankTag->GetAttribute("y")->GetValue();

            BattleCityTank tank(p.TankRadius * 2);
            tank.Pos.X = atoi(x);
            tank.Pos.Y = atoi(y);
            tank.Speed = tankSpeed;
            tank.Life = 10;
            StringHelper::Copy(textureValue, tank.TextureName, strlen(textureValue));
            p.Tanks.push_back(tank);

        }while(tanksTag->childTags.MoveNext());
    }

    /// Get walls tag
    Tag * wallsTag = rootTag->GetChildTag("Walls");

    /// Parse each wall tag
    if(!wallsTag->childTags.IsEmpty())
    {
        wallsTag->childTags.MoveFirst();

        do
        {
            Tag * wallTag = (Tag *)wallsTag->childTags.GetCurrent();

            char * x = wallTag->GetAttribute("x")->GetValue();
            char * y = wallTag->GetAttribute("y")->GetValue();
            char * width = wallTag->GetAttribute("width")->GetValue();
            char * height = wallTag->GetAttribute("height")->GetValue();
            char * typeStr = wallTag->GetAttribute("type")->GetValue();

            int type = atoi(typeStr);

            Rect rect;
	        rect.X = atoi(x);
	        rect.Y = atoi(y);
	        rect.Width = atoi(width);
	        rect.Height = atoi(height);

	        BattleCityWall wall ( rect , (BattleCityWallTypes)type );
	        p.Walls.push_back(wall);

        }while(wallsTag->childTags.MoveNext());
    }
	
	return p;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;

	try
	{
		BattleCityParameters p = ParseParameters("Arena01.xml");
		BattleCityServer srv ( BATTLE_CITY_SOCKET , p );
		srv.Start();
	}
	catch ( exception ex )
	{
		cerr << ex.what();
	}
    catch ( Exception ex )
    {
        printf(ex.GetMessage());
    }

	return retval;
}
