#ifndef BattleCityServer_cpp
#define BattleCityServer_cpp

#include "BattleCityServer.h"
#include "sdl_keysym.h"
#include "Screen.h"
#include "conio.h"
#include "iostream"
#include "Socket.h"

BattleCityServer::BattleCityServer(int port,BattleCityParameters parameters) 
	: port(port), salir(false), engine(NULL), parameters(parameters) , lastFrame(0)
{	
	for ( int i = 0 ; i < BATTLE_CITY_MAX_PLAYERS ; i++ )
		sockets[i] = SOCKET_ERROR;

	engine = new BattleCityEngine ( parameters );
	
    mutex = CreateMutex ( NULL , FALSE , NULL );

    this->numPlayersConnected = 0;
}

BattleCityServer::~BattleCityServer()
{
	if ( engine != NULL )
		delete engine;

	CloseHandle ( mutex );
}

int BattleCityServer::Start()
{
    CreateThread ( NULL , 0 , MainThread , (void*) this , 0 , NULL );

    int resultado = 0;

    Socket listeningSocket;

    listeningSocket.Listen(this->port, BATTLE_CITY_MAX_PLAYERS);

    while ( !salir)
	{
        printf("Waiting for clients.\n");

        Socket newSocket;

        if(listeningSocket.Accept(&newSocket) == RES_OK)
        {
            NewConnection(newSocket.GetConnection().cxSocket);
        }
	}

    if(listeningSocket.IsActive())
        listeningSocket.Close();

    return resultado;
}

struct BCThreadParam
{
	int socketPos;
	BattleCityServer* ptr;
};

void BattleCityServer::NewConnection(SOCKET s)
{
	int first = -1;
	for ( int i = 0 ; i < BATTLE_CITY_MAX_PLAYERS ; i++ )
		if ( sockets[i] == SOCKET_ERROR )
		{
			first = i;
			break;
		}

	if ( first == - 1 )
	{
		unsigned char msg [ 2 ];
		msg[0] = 0xFF;
		msg[1] = 0;
		send(s,(char*)msg,2,0);
		closesocket(s);
	}
	else
	{
		sockets[first] = s;

        /// Increment the number of players connected
        this->numPlayersConnected++;

        /// Print message to screen
        printf("Client %d connected on port %d.\n", first + 1, port);

        /// Send what number of player the client is
        BattleCityPlayerNumberPacket playerPacket(first);
        playerPacket.Send(sockets[first]);

        /// Send parameters to client
        BattleCityParametersPacket parametersPacket(GetBattleCityClientParameters(this->parameters));
        parametersPacket.Send(sockets[first]);

        /// Send all textures to client
        for(int textureIndex = 0; textureIndex < this->parameters.Textures.size();textureIndex++)
        {
            BattleCityTexture texture = this->parameters.Textures[textureIndex];
            BattleCityTexturePacket texturePacket(texture.GetName(), texture.GetPath());

            texturePacket.Send(sockets[first]);
        }

		BCThreadParam* p = new BCThreadParam();
		p->socketPos = first;
		p->ptr = this;
		CreateThread ( NULL , 0 , ThreadProc , (LPVOID) p , 0 , NULL );

        /// Send state for the first time
        this->UpdateClients(this->engine->GetState());
	}
}

void BattleCityServer::Stop()
{
	salir = true;
	//closesocket(sock);
	for ( int i = 0 ; i < BATTLE_CITY_MAX_PLAYERS ; i++ )
		closesocket(sockets[i]);
}

void BattleCityServer::OnKey(int tank, int tecla)
{
	if ( tecla == SDLK_LEFT )
		engine->TurnTank(tank, LEFT);
	else if ( tecla == SDLK_DOWN )
		engine->TurnTank(tank, DOWN);
	else if ( tecla == SDLK_RIGHT )
		engine->TurnTank(tank, RIGHT);
	else if ( tecla == SDLK_UP )
		engine->TurnTank(tank,UP);
	else if ( tecla == SDLK_c )
		engine->DropBomb(tank);
	else if ( tecla == SDLK_b )
		engine->ShootBullet(tank);
}

DWORD BattleCityServer::ThreadProc ( LPVOID param )
{
	BCThreadParam* p = (BCThreadParam*) param;
	bool salir = false;

    while ( !salir && !p->ptr->salir )
    {
        BattleCityDataPacket * dataPacket = (BattleCityDataPacket *) BattleCityCommunicationProtocol::ReceiveDataPacket(p->ptr->sockets[p->socketPos]);
        BattleCityCommandPacket * cmdPacket = NULL;

        if(dataPacket != NULL && dataPacket->GetType() == COMMAND)
        {
            cmdPacket = (BattleCityCommandPacket *) dataPacket;
            if(cmdPacket->GetCommandType() == KEYPRESSED)
            {
                WaitForSingleObject ( p->ptr->mutex , INFINITE );
                p->ptr->OnKey ( cmdPacket->GetClientNumber(), cmdPacket->GetAuxValue());
                ReleaseMutex ( p->ptr->mutex );
            }
        }

        delete dataPacket;
    }

	p->ptr->sockets[p->socketPos] = SOCKET_ERROR;
	delete p;
	return 0;
}

void UpdateEngine(BattleCityEngine& e, int tecla)
{
	if ( tecla == 'a' )
		e.TurnTank(0,LEFT);
	else if ( tecla == 's' )
		e.TurnTank(0,DOWN);
	else if ( tecla == 'd' )
		e.TurnTank(0,RIGHT);
	else if ( tecla == 'w' )
		e.TurnTank(0,UP);
	else if ( tecla == 'q' )
		e.DropBomb(0);
	else if ( tecla == 'e' )
		e.ShootBullet(0);
	else if ( tecla == 'j' )
		e.TurnTank(1,LEFT);
	else if ( tecla == 'k' )
		e.TurnTank(1,DOWN);
	else if ( tecla == 'l' )
		e.TurnTank(1,RIGHT);
	else if ( tecla == 'i' )
		e.TurnTank(1,UP);
	else if ( tecla == 'u' )
		e.DropBomb(1);
	else if ( tecla == 'o' )
		e.ShootBullet(1);
}


#define FRAME_INTERVAL	
#define DELAY_GUARD		0


DWORD BattleCityServer::MainThread(LPVOID param)
{
	BattleCityServer* p = (BattleCityServer*) param;

	WaitForSingleObject ( p->mutex , INFINITE );
    ///	RenderScreen(p->engine->GetState());
	p->engine->Start();
	ReleaseMutex ( p->mutex );

	bool salir = false;
	do
	{
		if ( kbhit() )
		{
			int tecla = getch();
			if ( tecla == 27 ) 
				salir = true;
			else 
			{
				WaitForSingleObject ( p->mutex , INFINITE );
				UpdateEngine(*p->engine,tecla);
				ReleaseMutex ( p->mutex );
			}
		}

		WaitForSingleObject ( p->mutex , INFINITE );
		p->engine->Tick();
		if ( p->engine->GetDirty() /*&& (p->lastFrame + FRAME_INTERVAL) < GetTickCount()*/ )
		{
			p->lastFrame = GetTickCount();
			BattleCityState state = p->engine->GetState();
			/// RenderScreen(state);
			p->UpdateClients(state);
		}
		ReleaseMutex ( p->mutex );

		if ( p->engine->GetFinished() )
		{
			Sleep ( 2000 );
			p->engine->Start();
		}
		else
		{
			WaitForSingleObject ( p->mutex , INFINITE );
			int delay = p->engine->GetNextTickInterval();
			ReleaseMutex ( p->mutex );

			if ( delay != 0 )
				Sleep ( delay );
			else
				Sleep ( DELAY_GUARD );
		}
	}
	while ( !salir && !p->salir );

	return 0;
}

void BattleCityServer::UpdateClients(BattleCityState state)
{
    BattleCityScenario scenario(this->parameters.ArenaWidth, this->parameters.ArenaHeight); 

    /************************************************/
    /* Send Tanks state                           */
    /************************************************/
    for(int tankIndex = 0; tankIndex < state.Tanks.size(); tankIndex++)
    {
        Rect quadrant;

        int i = 0;

        for(i = 0; i < state.Tanks.size(); i++)
        {
            /// Send all tanks even if they are not in the current
            /// client's quadrant, so the life of all other tanks is shown
            /// in every tank's screen.
            vector<BattleCityTank> tanks;
            tanks.push_back(state.Tanks[i]);

            BattleCityTankPacket tankPacket(tanks);
            this->SendToClient(&tankPacket, sockets[tankIndex]);

            if(i == tankIndex)
            {
                quadrant = scenario.GetQuadrant(state.Tanks[tankIndex].Pos);
            }
        }

        /************************************************/
        /* Send Bullets state                           */
        /************************************************/
        for(i = 0; i < state.Bullets.size(); i++)
        {
            vector<BattleCityBullet> bullets;

            if(state.Bullets[i].Intersects(quadrant))
            {
                bullets.push_back(state.Bullets[i]);
            }

            BattleCityBulletPacket bulletPacket(bullets);
            this->SendToClient(&bulletPacket, sockets[tankIndex]);
        }

        /************************************************/
        /* Send Bombs state                           */
        /************************************************/
        for(i = 0; i < state.Bombs.size(); i++)
        {
            vector<BattleCityBomb> bombs;
            if(state.Bombs[i].Intersects(quadrant))
            {
                bombs.push_back(state.Bombs[i]);
            }
            BattleCityBombPacket bombPacket(bombs);
            this->SendToClient(&bombPacket, sockets[tankIndex]);
        }

        /************************************************/
        /* Send Walls state                           */
        /************************************************/
        for(i = 0; i < state.Walls.size(); i++)
        {
            vector<BattleCityWall> walls;
            if(state.Walls[i].Intersects(quadrant))
            {
                walls.push_back(state.Walls[i]);
            }
            BattleCityWallPacket wallPacket(walls);
            this->SendToClient(&wallPacket, sockets[tankIndex]);
        }
    }

    /************************************************/
    /* Update screen                                */
    /************************************************/
    BattleCityCommandPacket cmdPacket(UPDATESCREEN);
    this->SendToAllClients(&cmdPacket);
}

void BattleCityServer::SendToAllClients(BattleCityDataPacket * packet)
{
	for ( int i = 0 ; i < BATTLE_CITY_MAX_PLAYERS ; i++ )
    {
        this->SendToClient(packet, sockets[i]);
    }
}

void BattleCityServer::SendToClient(BattleCityDataPacket * packet, SOCKET sock)
{
    if ( sock != SOCKET_ERROR )
    {
        packet->Send(sock);
    }
}

#endif