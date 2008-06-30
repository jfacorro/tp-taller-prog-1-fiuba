#ifndef BattleCityClient_cpp
#define BattleCityClient_cpp

#include "BattleCityClient.h"
#include "Screen.h"
#include "Socket.h"
#include "BattleCityCommunicationProtocol.h"
#include "iostream"
#include "conio.h"
#include "SDL_ttf.h"

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

    this->sdlHelper.Initialize();
    TTF_Init();
    atexit(TTF_Quit);
    this->sdlFont = TTF_OpenFont("fonts\\cour.ttf", 14);

    /// First get the parameters from the server
    this->ReceiveParametersAndTextures();

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

    TTF_CloseFont(this->sdlFont);
}

void BattleCityClient::ReceiveParametersAndTextures()
{
    bool salir = false;

    while(!salir)
    {
        BattleCityDataPacket * packet = (BattleCityDataPacket *)BattleCityCommunicationProtocol::ReceiveDataPacket(this->socket);

        if(packet != NULL)
        {
            BattleCityClient::ProcessPacket(this, packet);

            salir = (packet->GetType() == COMMAND);
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
		
		int i = 0;

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
                for(i = 0; i < tankPacket->tanks.size(); i++)
                {
                    client->state.Tanks.push_back(tankPacket->tanks[i]);
                }
                break;
            case BULLET:
                BattleCityCommunicationProtocol::WriteLog("Received BULLET.\n");
                bulletPacket = (BattleCityBulletPacket *)packet;
                for(i = 0; i < bulletPacket->bullets.size(); i++)
                {
                    client->state.Bullets.push_back(bulletPacket->bullets[i]);
                }
                break;
            case BOMB:
                BattleCityCommunicationProtocol::WriteLog("Received BOMB.\n");
                bombPacket = (BattleCityBombPacket *)packet;
                for(i = 0; i < bombPacket->bombs.size(); i++)
                {
                    client->state.Bombs.push_back(bombPacket->bombs[i]);
                }
                break;
            case WALL:
                BattleCityCommunicationProtocol::WriteLog("Received WALL.\n");
                wallPacket = (BattleCityWallPacket *)packet;
                for(i = 0; i < wallPacket->walls.size(); i++)
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
        config.SetResolucion(SDLHelper::ResolutionByWidth(800));
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

    SDL_Color fontColor = {0,0,0,0};  // Blue ("Fg" is foreground)

	Configuration config = this->sdlHelper.GetConfiguration();

	int bombBlastRadius = this->parameters.BombBlastRadius;
    int pixelsPerUM = this->parameters.PixelsPerUM;

    Rect quadrant;
    bool receivedLocalTank = false;

	int i = 0;
    
    for(i = 0; i <state.Tanks.size(); i++)    
    {
        if(this->clientNumber == i)
        {
            quadrant = scenario.GetQuadrant(state.Tanks[i].Pos);
            receivedLocalTank = true;
        }
    }

    /// Just in case the tank was not sent
    if(!receivedLocalTank) return;

    SDL_Surface * background = NULL;

    background = this->GetTexture(this->parameters.BackGroundTextureId);

    if(background != NULL)
    {
        background = SDLHelper::SDLResizeBitmap(background, config.GetResolucion().w * 2,  config.GetResolucion().h * 2);
        this->sdlHelper.DrawRectangle(0, 0, config.GetResolucion().w, config.GetResolucion().h, white, background, NULL);
        SDL_FreeSurface(background);
    }
    else
    {
	    this->sdlHelper.DrawRectangle(0, 0, config.GetResolucion().w, config.GetResolucion().h, white, NULL, NULL);
    }

	/************************************************/
    /// Draw Tanks
    /************************************************/
    for(i = 0; i <state.Tanks.size(); i++)
	{
        /// All tanks are sent to all clients in order
        /// to show life and points
        if(state.Tanks[i].Intersects(quadrant))
        {
            Rect tankRect = state.Tanks[i].GetRect();

            tankRect = this->GetScaledRectangle(tankRect, quadrant, pixelsPerUM);

            SDL_Surface * bitmap = SDLHelper::SDLResizeBitmap
            (
                this->GetTexture(state.Tanks[i].TextureName), 
                tankRect.Width, 
                tankRect.Height
            );

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
                bitmap = this->GetTexture(this->parameters.ExplosionTextureId);
                
                tankRect.Width = bitmap->w;
                tankRect.Height = bitmap->h;
                tankRect.X = state.Tanks[i].Pos.X - tankRect.Width / 2;
                tankRect.Y = state.Tanks[i].Pos.Y - tankRect.Height / 2;

                tankRect = this->GetScaledRectangle(tankRect, quadrant, pixelsPerUM);

                bitmap = SDLHelper::SDLResizeBitmap(bitmap, tankRect.Width, tankRect.Height);                
            }

            this->sdlHelper.DrawRectangle
            (
                tankRect.X,
                tankRect.Y,
                tankRect.Width,
                tankRect.Height,
                black, 
                bitmap, 
                NULL
            );

            if(bitmap != NULL) SDL_FreeSurface(bitmap);
        }

        /// Draw all lifes
        int infoX = config.GetResolucion().w - 100;
        int infoY = 15 * i + 5;
        this->sdlHelper.DrawRectangle(infoX, infoY, state.Tanks[i].Life * 5, 10, greenLife, NULL, NULL);

        /// Print points
        infoY += 50;
        SDL_Surface * sText = TTF_RenderText_Solid( this->sdlFont, "Points:", fontColor );
        this->sdlHelper.DrawRectangle(infoX, infoY, sText->w, sText->h, greenLife, sText, NULL);

        infoX += sText->w + 5;

        SDL_FreeSurface( sText );

        char pointsStr[10];
        itoa(state.Tanks[i].Points, pointsStr, 10);

        sText = TTF_RenderText_Solid( this->sdlFont, pointsStr, fontColor );
        this->sdlHelper.DrawRectangle(infoX, infoY, sText->w, sText->h, greenLife, sText, NULL);
        SDL_FreeSurface( sText );
	}

	/************************************************/
    /// Draw Bombs
    /************************************************/
    for(i = 0; i <state.Bombs.size(); i++)
	{
        Rect bombRect = state.Bombs[i].GetRect();

        bombRect = this->GetScaledRectangle(bombRect, quadrant, pixelsPerUM);

        if ( state.Bombs[i].TimeToDie >= 0 )
		{
			this->sdlHelper.DrawRectangle
            (
                bombRect.X ,
                bombRect.Y,
                bombRect.Width,
                bombRect.Height,
                blue,
                NULL, 
                NULL
            );
        }
		else
		{
			this->sdlHelper.DrawCircle
            (
                (state.Bombs[i].Pos.X - quadrant.X) * pixelsPerUM,
                (state.Bombs[i].Pos.Y - quadrant.Y) * pixelsPerUM,
                bombBlastRadius * pixelsPerUM,
                red,
                NULL, 
                NULL
            );
		}
	}

	/************************************************/
    /// Draw Bullets
    /************************************************/
    for (i = 0 ; i < state.Bullets.size() ; i++ )
	{
        Rect bulletRect = state.Bullets[i].GetRect();

        bulletRect = this->GetScaledRectangle(bulletRect, quadrant, pixelsPerUM);

		this->sdlHelper.DrawRectangle
        (
            bulletRect.X,
            bulletRect.Y,
            bulletRect.Width,
            bulletRect.Height,
            green, 
            NULL, 
            NULL
        );
	}

	/************************************************/
    /// Draw Walls
    /************************************************/
    for ( unsigned int j = 0 ; j < state.Walls.size() ; j++ )
	{
		Rect rect = state.Walls[j].GetRect();

        rect = this->GetScaledRectangle(rect, quadrant, pixelsPerUM);

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
		
		this->sdlHelper.DrawRectangle
        (
            rect.X, 
            rect.Y,
            rect.Width,
            rect.Height,
            wallColor, 
            bitmap, 
            NULL
        );

        if(bitmap != NULL) SDL_FreeSurface(bitmap);
	}

	this->sdlHelper.Refresh();
}

Rect BattleCityClient::GetScaledRectangle(Rect source, Rect quadrant, int pixelsPerUM)
{
    Rect rect;

    int x = source.X + source.Width / 2;
    int y = source.Y + source.Height / 2;

    rect.Width = source.Width * pixelsPerUM;
    rect.Height = source.Height * pixelsPerUM;
    rect.X = (x - quadrant.X) * pixelsPerUM - rect.Width / 2;
    rect.Y = (y - quadrant.Y) * pixelsPerUM - rect.Height / 2;

    return rect;
}

void BattleCityClient::RenderScreenChars()
{
	clrscr();

    int i = 0;

	for(i = 0; i <state.Tanks.size(); i++)
    {
	    gotoxy((int)state.Tanks[i].Pos.X,(int)state.Tanks[i].Pos.Y); cout << "O";
    }

    for (i = 0 ; i < state.Bombs.size() ; i++ )
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

	for (i = 0 ; i < state.Bullets.size() ; i++ )
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
