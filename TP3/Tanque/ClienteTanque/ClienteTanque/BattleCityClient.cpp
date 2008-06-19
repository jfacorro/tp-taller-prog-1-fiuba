#ifndef BattleCityClient_cpp
#define BattleCityClient_cpp

#include "BattleCityClient.h"
#include "Screen.h"
#include "BattleCityCommunicationProtocol.h"
#include "iostream"
#include "conio.h"

void BattleCityClient::Connect(char * dir, int socketNumber)
{
    int resultado = this->socket.Connect("127.0.0.1", BATTLE_CITY_SOCKET);

    this->isConnected = (resultado == RES_OK);
}

void BattleCityClient::StartPlaying()
{
	bool salir = false;

	this->sdlHelper.Initialize();

    while ( !salir )
	{
		SDL_keysym keyPressed;
		if(this->sdlHelper.GetPressedKey(keyPressed))
		{
			if(keyPressed.sym == SDLK_ESCAPE)
			{
				salir = true;
			}
			else
			{
				UpdateEngine(keyPressed.sym);
			}
		}

        BattleCityDataPacket * packet = (BattleCityDataPacket *)BattleCityCommunicationProtocol::ReceiveDataPacket(this->socket);

        BattleCityPlayerNumberPacket * playerNumberPacket  = NULL;
        BattleCityParametersPacket * parametersPacket = NULL;
        BattleCityCommandPacket * cmdPacket  = NULL;
        BattleCityTankPacket * tankPacket = NULL;
        BattleCityBulletPacket * bulletPacket = NULL;
        BattleCityBombPacket * bombPacket = NULL;
        BattleCityWallPacket * wallPacket = NULL;

        if(packet != NULL)
        {
            switch(packet->GetType())
            {
                case DUMMY:
                    break;
                case PLAYERNUMBER:
                    playerNumberPacket = (BattleCityPlayerNumberPacket *)packet;
                    this->clientNumber = playerNumberPacket->GetPlayerNumber();
                    break;
                case PARAMETERS:
                    parametersPacket = (BattleCityParametersPacket *)packet;
                    this->parameters = parametersPacket->GetParameters();
                    break;
                case TANK:
                    tankPacket = (BattleCityTankPacket *)packet;
                    for(int i = 0; i < tankPacket->tanks.size(); i++)
                    {
                        this->state.Tanks.push_back(tankPacket->tanks[i]);
                    }
                    break;
                case BULLET:
                    bulletPacket = (BattleCityBulletPacket *)packet;
                    for(int i = 0; i < bulletPacket->bullets.size(); i++)
                    {
                        this->state.Bullets.push_back(bulletPacket->bullets[i]);
                    }
                    break;
                case BOMB:
                    bombPacket = (BattleCityBombPacket *)packet;
                    for(int i = 0; i < bombPacket->bombs.size(); i++)
                    {
                        this->state.Bombs.push_back(bombPacket->bombs[i]);
                    }
                    break;
                case WALL:
                    wallPacket = (BattleCityWallPacket *)packet;
                    for(int i = 0; i < wallPacket->walls.size(); i++)
                    {
                        this->state.Walls.push_back(wallPacket->walls[i]);
                    }
                    break;
                case COMMAND:
                    cmdPacket = (BattleCityCommandPacket *)packet;
                    if(cmdPacket->GetCommandType() == UPDATESCREEN)
                    {
                        RenderScreenSDL();

                        this->state.Tanks.clear();
                        this->state.Bullets.clear();
                        this->state.Bombs.clear();
                        this->state.Walls.clear();
                    }

                    break;
            }
        }

        delete packet;
	}
}

void BattleCityClient::UpdateEngine (int tecla)
{
    SocketPacket keyPacket(6);

    char * buffer = keyPacket.GetData();

	buffer[0] = 2;
	buffer[1] = 0;
	buffer[2] = 6;
	buffer[3] = 0;
	buffer[4] = (tecla & 0xFF);
	buffer[5] = (tecla >> 8);

	send(this->socket.GetConnection().cxSocket, (char*)buffer, 6, 0);
}

void BattleCityClient::RenderScreenSDL()
{
	if(!this->sdlHelper.VideoInitialized())
	{
		Configuration config;
		config.SetResolucion(SDLHelper::ResolutionByWidth(this->parameters.ArenaWidth));
		this->sdlHelper.InitializeVideo(config);
	}

	Color white;
	white.R = white.G = white.B = 255;
	Color black;
	black.R = black.G = black.B = 0;
	Color blue;
	blue.R = blue.G = 0;
	blue.B = 255;
	Color red;
	red.B = red.G = 0;
	red.R = 255;
	Color green;
	green.B = green.R = 0;
	green.G = 255;
	Color greyIron;
	greyIron.B = greyIron.R = greyIron.G = 200;
	Color greyWood;
	greyWood.B = greyWood.R = greyWood.G = 150;
	Color greyRock;
	greyRock.B = greyRock.R = greyRock.G = 100;

	Configuration config = this->sdlHelper.GetConfiguration();

	this->sdlHelper.DrawRectangle(0, 0, config.GetResolucion().w, config.GetResolucion().h, white, NULL, NULL);
    
	int bombBlastRadius = this->parameters.BombBlastRadius;

	for(int i = 0; i <state.Tanks.size(); i++)
	{
        Rect tankRect = state.Tanks[i].GetRect();
		this->sdlHelper.DrawRectangle(tankRect.X, tankRect.Y, tankRect.Width, tankRect.Height, black, NULL, NULL);
	}

	for(int i = 0; i <state.Bombs.size(); i++)
	{
		if ( state.Bombs[i].TimeToDie >= 0 )
		{
            Rect bombRect = state.Bombs[i].GetRect();
			this->sdlHelper.DrawRectangle(bombRect.X, bombRect.Y, bombRect.Width, bombRect.Height, blue, NULL, NULL);
        }
		else
		{
			this->sdlHelper.DrawCircle((int)state.Bombs[i].Pos.X, (int)state.Bombs[i].Pos.Y, bombBlastRadius, red, NULL, NULL);
		}
	}

	for ( int i = 0 ; i < state.Bullets.size() ; i++ )
	{
        Rect bulletRect = state.Bullets[i].GetRect();
		this->sdlHelper.DrawRectangle(bulletRect.X, bulletRect.Y, bulletRect.Width, bulletRect.Height, green, NULL, NULL);
	}

	for ( unsigned int j = 0 ; j < state.Walls.size() ; j++ )
	{
		Rect rect = state.Walls[j].GetRect();
		BattleCityWallTypes t = state.Walls[j].GetType();
		Color wallColor;

		switch(t)
		{
			case WOOD:
				wallColor = greyWood;
				break;
			case IRON:
				wallColor = greyIron;
				break;
			case ROCK:
				wallColor = greyRock;
				break;
		}
		
		this->sdlHelper.DrawRectangle(rect.X, rect.Y, rect.Width, rect.Height, wallColor, NULL, NULL);
	}

	this->sdlHelper.Refresh();
}

void BattleCityClient::RenderScreenChars()
{
	clrscr();

    for(int i = 0; i <state.Tanks.size(); i++)
    {
	    gotoxy((int)state.Tanks[i].Pos.X,(int)state.Tanks[i].Pos.Y); cout << "O";
    }

    for ( unsigned int i = 0 ; i < state.Bombs.size() ; i++ )
    {
		if ( state.Bombs[i].TimeToDie >= 0 )
		{
			gotoxy(state.Bombs[i].Pos.X, state.Bombs[i].Pos.Y);
			cout << "B";
		}
		else
		{
			gotoxy(state.Bombs[i].Pos.X, state.Bombs[i].Pos.Y-1);
			cout << "*";
			gotoxy(state.Bombs[i].Pos.X-1, state.Bombs[i].Pos.Y);
			cout << "***";
			gotoxy(state.Bombs[i].Pos.X, state.Bombs[i].Pos.Y+1);
			cout << "*";
		}
    }

	for ( unsigned int i = 0 ; i < state.Bullets.size() ; i++ )
	{
		gotoxy((int)state.Bullets[i].Pos.X,(int)state.Bullets[i].Pos.Y);
		printf("*");
	}

	for ( unsigned int j = 0 ; j < state.Walls.size() ; j++ )
	{
		Rect rect = state.Walls[j].GetRect();
		for ( int a = rect.X ; a < (rect.X + rect.Width) ; a++ )
			for ( int b = rect.Y ; b < (rect.Y + rect.Height) ; b++ )
			{
				gotoxy(a,b);
				int life = state.Walls[j].GetLife();
				BattleCityWallTypes t = state.Walls[j].GetType();

				if ( t == WOOD )
				{
					if ( life == 1 )
						printf("w");
					else 
						printf("W");
				}
				else if ( t == ROCK )
				{
					if ( life == 1 )
						printf("r");
					else 
						printf("R");
				}
				else if ( t == IRON )
					printf ( "I" );
			}
	}
    /*
    for(int y = 0; y <state.Tanks.size(); y++)
    {
	    gotoxy ( 1 , y + 1 );
        for ( unsigned int i = 0 ; i < state.Tanks[y].Life ; i++ ) printf ( "x" );
    }
    */
}
#endif