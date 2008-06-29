#ifndef BattleCityClient_cpp
#define BattleCityClient_cpp

#include "BattleCityClient.h"
#include "Screen.h"
#include "Socket.h"
#include "BattleCityCommunicationProtocol.h"
#include "iostream"
#include "conio.h"

void BattleCityClient::InitializeTicks()
{
	this->lastTick = GetTickCount();
	this->nextTick = this->lastTick + 1000.0 / FRAMES_PER_SECOND;
}

void BattleCityClient::UpdateTicks()
{
	this->lastTick = this->nextTick;
	this->nextTick += 1000.0 / FRAMES_PER_SECOND;
}

void BattleCityClient::Connect(char * dir, int socketNumber)
{
    int resultado = this->socket.Connect(dir, BATTLE_CITY_SOCKET);

    this->isConnected = (resultado == RES_OK);
}

void BattleCityClient::StartPlaying()
{
    BattleCityCommunicationProtocol::WriteLog("---------------------------------------.\n");
	bool salir = false;	

    /// First get the parameters from the server
    this->ReceiveParameters();

    /// Initialize Video
    this->sdlHelper.Initialize();
    Configuration config;
    config.SetResolucion(SDLHelper::ResolutionByWidth(this->parameters.ArenaWidth / 2));
    this->sdlHelper.InitializeVideo(config);

    /// Initialize Counter
    this->InitializeTicks();

    /// Start receiving data and rendering thread
    CreateThread (NULL, 0, ReceiveDataFromServer, (void *) this, 0, NULL);

    /// Handle key presses
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
	}
}

void BattleCityClient::ReceiveParameters()
{
    bool salir = false;

    while(!salir)
    {
        BattleCityDataPacket * packet = (BattleCityDataPacket *)BattleCityCommunicationProtocol::ReceiveDataPacket(this->socket);

        BattleCityParametersPacket * parametersPacket = NULL;

        if(packet != NULL)
        {
            BattleCityClient::ProcessPacket(this, packet);

            salir = (packet->GetType() == PARAMETERS);
        }

        delete packet;
    }
}

DWORD __stdcall BattleCityClient::ReceiveDataFromServer ( LPVOID param )
{
    bool salir = false;

    BattleCityClient * client = (BattleCityClient *)param;

    while(!salir)
    {
        BattleCityDataPacket * packet = (BattleCityDataPacket *)BattleCityCommunicationProtocol::ReceiveDataPacket(client->socket);

        BattleCityClient::ProcessPacket(client, packet);

        delete packet;
    }

    return 0;
}

void BattleCityClient::ProcessPacket(BattleCityClient * client, BattleCityDataPacket * packet)
{
    BattleCityPlayerNumberPacket * playerNumberPacket  = NULL;
    BattleCityParametersPacket * parametersPacket = NULL;
    BattleCityCommandPacket * cmdPacket  = NULL;
    BattleCityTankPacket * tankPacket = NULL;
    BattleCityBulletPacket * bulletPacket = NULL;
    BattleCityBombPacket * bombPacket = NULL;
    BattleCityWallPacket * wallPacket = NULL;
    BattleCityTexturePacket * texturePacket = NULL;

    if(packet != NULL)
    {
        WaitForSingleObject(client->mutex, INFINITE);

        switch(packet->GetType())
        {
            case DUMMY:
                BattleCityCommunicationProtocol::WriteLog("Received DUMMY.\n");
                break;
            case PLAYERNUMBER:
                BattleCityCommunicationProtocol::WriteLog("Received PLAYERNUMBER.\n");
                playerNumberPacket = (BattleCityPlayerNumberPacket *)packet;
                client->clientNumber = playerNumberPacket->GetPlayerNumber();
                break;
            case PARAMETERS:
                BattleCityCommunicationProtocol::WriteLog("Received PARAMETERS.\n");
                parametersPacket = (BattleCityParametersPacket *)packet;
                client->parameters = parametersPacket->GetParameters();
                break;
            case TANK:
                BattleCityCommunicationProtocol::WriteLog("Received TANK.\n");
                tankPacket = (BattleCityTankPacket *)packet;
                for(int i = 0; i < tankPacket->tanks.size(); i++)
                {
                    client->state.Tanks.push_back(tankPacket->tanks[i]);
                }
                break;
            case BULLET:
                BattleCityCommunicationProtocol::WriteLog("Received BULLET.\n");
                bulletPacket = (BattleCityBulletPacket *)packet;
                for(int i = 0; i < bulletPacket->bullets.size(); i++)
                {
                    client->state.Bullets.push_back(bulletPacket->bullets[i]);
                }
                break;
            case BOMB:
                BattleCityCommunicationProtocol::WriteLog("Received BOMB.\n");
                bombPacket = (BattleCityBombPacket *)packet;
                for(int i = 0; i < bombPacket->bombs.size(); i++)
                {
                    client->state.Bombs.push_back(bombPacket->bombs[i]);
                }
                break;
            case WALL:
                BattleCityCommunicationProtocol::WriteLog("Received WALL.\n");
                wallPacket = (BattleCityWallPacket *)packet;
                for(int i = 0; i < wallPacket->walls.size(); i++)
                {
                    client->state.Walls.push_back(wallPacket->walls[i]);
                }
                break;
            case TEXTURE:
                BattleCityCommunicationProtocol::WriteLog("Received TEXTURE.\n");
                texturePacket = (BattleCityTexturePacket *)packet;
                client->AddTexture(texturePacket->GetBitmapName(), texturePacket->SaveBitmap());
                break;
            case COMMAND:
                BattleCityCommunicationProtocol::WriteLog("Received COMMAND:");
                cmdPacket = (BattleCityCommandPacket *)packet;
                if(cmdPacket->GetCommandType() == UPDATESCREEN)
                {
                    BattleCityCommunicationProtocol::WriteLog(" UPDATESCREEN.\n");
                    client->RenderScreenSDL();

				    client->UpdateTicks();

                    client->state.Tanks.clear();
                    client->state.Bullets.clear();
                    client->state.Bombs.clear();
                    client->state.Walls.clear();
                }

                break;
        }

        ReleaseMutex(client->mutex);
    }
}

void BattleCityClient::AddTexture(char * name, char * filename)
{
    Texture * texture = new Texture(name, filename);

    this->textures.push_back(texture);
}

SDL_Surface * BattleCityClient::GetTexture(char * name)
{
    for(int i = 0; i < this->textures.size(); i++)
    {
        if(strcmp(this->textures[i]->GetId(), name) == 0)
        {
            return this->textures[i]->GetBitmap();
        }
    }

    return NULL;
}

void BattleCityClient::UpdateEngine (int tecla)
{
    #ifdef DEBUG
    /// printf("Key was pressed = %d\n", tecla);
    #endif

	BattleCityCommandPacket keyPacket(KEYPRESSED, this->clientNumber, tecla);
    keyPacket.Send(this->socket.GetConnection().cxSocket);
}

void BattleCityClient::RenderScreenSDL()
{
    /************************************************/
    /// Check if video is initialized
    /************************************************/
    if(!this->sdlHelper.VideoInitialized())
	{
		Configuration config;
		config.SetResolucion(SDLHelper::ResolutionByWidth(this->parameters.ArenaWidth / 2));
		this->sdlHelper.InitializeVideo(config);
	}

    BattleCityScenario scenario(this->parameters.ArenaWidth, this->parameters.ArenaHeight); 

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
    Color greenLife;
	greenLife.B = greenLife.R = 100;
	greenLife.G = 255;
	Color greyIron;
	greyIron.B = greyIron.R = greyIron.G = 200;
	Color greyWood;
	greyWood.B = greyWood.R = greyWood.G = 150;
	Color greyRock;
	greyRock.B = greyRock.R = greyRock.G = 100;

	Configuration config = this->sdlHelper.GetConfiguration();

	this->sdlHelper.DrawRectangle(0, 0, config.GetResolucion().w, config.GetResolucion().h, white, NULL, NULL);

	int bombBlastRadius = this->parameters.BombBlastRadius;

    Rect quadrant;
    bool receivedLocalTank = false;
    
    for(int i = 0; i <state.Tanks.size(); i++)    
    {
        if(this->clientNumber == i)
        {
            quadrant = scenario.GetQuadrant(state.Tanks[i].Pos);
            receivedLocalTank = true;
        }
    }

    /// Just in case the tank was not sent
    if(!receivedLocalTank) return;

	/************************************************/
    /// Draw Tanks
    /************************************************/
    for(int i = 0; i <state.Tanks.size(); i++)
	{
        /// All tanks are sent to all clients in order
        /// to show life and points
        if(state.Tanks[i].Intersects(quadrant))
        {
            Rect tankRect = state.Tanks[i].GetRect();

            SDL_Surface * bitmap = SDLHelper::SDLResizeBitmap(this->GetTexture(state.Tanks[i].TextureName), tankRect.Width, tankRect.Height);

            if(state.Tanks[i].Life > 0)
            {
                SDL_Surface * rotatedbitmap = NULL;

                switch(state.Tanks[i].Direction)
                {
                    case LEFT:
                        rotatedbitmap = SDLHelper::SDLRotateBitmap(bitmap, 90);
                        break;
                    case RIGHT:
                        rotatedbitmap = SDLHelper::SDLRotateBitmap(bitmap, -90);
                        break;
                    case NONE:
                    case UP:
                        rotatedbitmap = SDLHelper::SDLRotateBitmap(bitmap, 0);
                        break;
                    case DOWN:
                        rotatedbitmap = SDLHelper::SDLRotateBitmap(bitmap, 180);
                        break;
                }

                if(bitmap != NULL) SDL_FreeSurface(bitmap);

                bitmap = rotatedbitmap;
            }
            else
            {
                bitmap = this->GetTexture("explosion01");
                bitmap = SDLHelper::SDLResizeBitmap(bitmap, bitmap->w, bitmap->h);
            }

            this->sdlHelper.DrawRectangle(tankRect.X - quadrant.X, tankRect.Y - quadrant.Y, tankRect.Width, tankRect.Height, black, bitmap, NULL);

            if(bitmap != NULL) SDL_FreeSurface(bitmap);
        }
        /// Draw all lifes
        this->sdlHelper.DrawRectangle(config.GetResolucion().w - 150, 15 * i + 5, state.Tanks[i].Life * 5, 10, greenLife, NULL, NULL);

        printf("Tank %d point's: %d\n", i+1, state.Tanks[i].Points);
	}

	/************************************************/
    /// Draw Bombs
    /************************************************/
    for(int i = 0; i <state.Bombs.size(); i++)
	{
		if ( state.Bombs[i].TimeToDie >= 0 )
		{
            Rect bombRect = state.Bombs[i].GetRect();
			this->sdlHelper.DrawRectangle(bombRect.X - quadrant.X, bombRect.Y - quadrant.Y, bombRect.Width, bombRect.Height, blue, NULL, NULL);
        }
		else
		{
			this->sdlHelper.DrawCircle((int)state.Bombs[i].Pos.X - quadrant.X, (int)state.Bombs[i].Pos.Y - quadrant.Y, bombBlastRadius, red, NULL, NULL);
		}
	}

	/************************************************/
    /// Draw Bullets
    /************************************************/
    for ( int i = 0 ; i < state.Bullets.size() ; i++ )
	{
        Rect bulletRect = state.Bullets[i].GetRect();
		this->sdlHelper.DrawRectangle(bulletRect.X - quadrant.X, bulletRect.Y - quadrant.Y, bulletRect.Width, bulletRect.Height, green, NULL, NULL);
	}

	/************************************************/
    /// Draw Walls
    /************************************************/
    for ( unsigned int j = 0 ; j < state.Walls.size() ; j++ )
	{
		Rect rect = state.Walls[j].GetRect();
		BattleCityWallTypes t = state.Walls[j].GetType();
		Color wallColor;

        SDL_Surface * bitmap = NULL;

		switch(t)
		{
			case WOOD:
				wallColor = greyWood;
                bitmap = SDLHelper::SDLResizeBitmap(this->GetTexture("wood"), rect.Width, rect.Height);
				break;
			case IRON:
				wallColor = greyIron;
                bitmap = SDLHelper::SDLResizeBitmap(this->GetTexture("iron"), rect.Width, rect.Height);
				break;
			case ROCK:
				wallColor = greyRock;
                bitmap = SDLHelper::SDLResizeBitmap(this->GetTexture("rock"), rect.Width, rect.Height);
				break;
		}
		
		this->sdlHelper.DrawRectangle(rect.X - quadrant.X, rect.Y - quadrant.Y, rect.Width, rect.Height, wallColor, bitmap, NULL);

        if(bitmap != NULL) SDL_FreeSurface(bitmap);
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