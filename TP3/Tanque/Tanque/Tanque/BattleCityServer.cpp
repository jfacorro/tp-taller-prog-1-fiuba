#ifndef BattleCityServer_cpp
#define BattleCityServer_cpp

#include "BattleCityServer.h"
#include "sdl_keysym.h"
#include "Screen.h"
#include "conio.h"
#include "iostream"

BattleCityServer::BattleCityServer(int port,BattleCityParameters parameters) 
	: port(port), salir(false), engine(NULL), parameters(parameters)
{	
	for ( int i = 0 ; i < BATTLE_CITY_MAX_PLAYERS ; i++ )
		sockets[i] = SOCKET_ERROR;

	engine = new BattleCityEngine ( parameters );
	mutex = CreateMutex ( NULL , FALSE , NULL );
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

   WSADATA wsaData; /* Utilizada para inicializacion del winsock */
   WORD wVersionReq = MAKEWORD(2, 2); /* Contiene la version */
   SOCKET sockAceptado; /* Contiene la informacion del socket que acepto */
   //struct hostent* hostInfo; /* Se utiliza para convertir el nombre del host a su direccion IP */
   struct sockaddr_in sockAddrIn; /* Direccion de socket */
   int tamSockAddrIn; /* Se utiliza para aceptar el mensaje y obtener su info */
   int codError = 0; /* Codigo de error que se evalua dentro de la funcion */
   
   int resultado = RES_ERROR_UNKNOWN; /* Codigo de error que se devolvera */

   /* Inicializa el socket */
   codError = WSAStartup(wVersionReq, &wsaData);
   if (codError != 0) {
      resultado = RES_ERROR_STARTUP;

   } else {
      /* Crea un socket */
      sock = socket(AF_INET, SOCK_STREAM, 0);
      if (sock == INVALID_SOCKET) {
         resultado = RES_INVALID_SOCKET;
         
      } else {
         /* Carga los valores de sockAddrIn */
         sockAddrIn.sin_family = AF_INET;     /* Protocolo ipv4 */
         sockAddrIn.sin_port = htons(port); /* Puerto indicado */
         sockAddrIn.sin_addr.s_addr = INADDR_ANY; /* Cualquier direccion IP */
         
         codError = bind(sock, (sockaddr*) &sockAddrIn, sizeof(sockAddrIn));
         if (codError != 0) {
            resultado = RES_ERROR_BIND;

         } else {
            /* Escucha una sola conexion */
            codError = listen(sock, 5);
            if (codError != 0) {
               resultado = RES_ERROR_LISTEN;
            } else {

				while ( !salir )
				{
					/* Acepta la conexion */
					sockAceptado = SOCKET_ERROR;
					tamSockAddrIn = sizeof(sockAddrIn);
					sockAceptado = accept(sock, (sockaddr*) &sockAddrIn, &tamSockAddrIn);

					if ( sockAceptado != SOCKET_ERROR )
						NewConnection(sockAceptado);
				}

				resultado = RES_OK;
            }
         }
      }
   }

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

        BattleCityPlayerNumberPacket playerPacket(first);
        playerPacket.Send(s);

        BattleCityParametersPacket parametersPacket(GetBattleCityClientParameters(this->parameters));
        parametersPacket.Send(s);

		BCThreadParam* p = new BCThreadParam();
		p->socketPos = first;
		p->ptr = this;
		CreateThread ( NULL , 0 , ThreadProc , (LPVOID) p , 0 , NULL );
	}
}

void BattleCityServer::Stop()
{
	salir = true;
	closesocket(sock);
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

void RenderScreen(BattleCityState& state)
{
	return;

	clrscr();
	gotoxy((int)state.Tanks[0].Pos.X,(int)state.Tanks[0].Pos.Y); cout << "O";
	gotoxy((int)state.Tanks[1].Pos.X,(int)state.Tanks[1].Pos.Y); cout << "O";

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

	gotoxy ( 1 , 1 );
	for ( unsigned int i = 0 ; i < state.Tanks[0].Life ; i++ ) printf ( "x" );
	gotoxy ( 1 , 2 );
	for ( unsigned int i = 0 ; i < state.Tanks[1].Life ; i++ ) printf ( "x" );
}



DWORD BattleCityServer::MainThread(LPVOID param)
{
	BattleCityServer* p = (BattleCityServer*) param;

	WaitForSingleObject ( p->mutex , INFINITE );
	RenderScreen(p->engine->GetState());
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
		if ( p->engine->GetDirty() )
		{
			BattleCityState state = p->engine->GetState();
			RenderScreen(state);
			p->UpdateClients(state);
		}
		ReleaseMutex ( p->mutex );

		WaitForSingleObject ( p->mutex , INFINITE );
		int delay = p->engine->GetNextTickInterval();
		ReleaseMutex ( p->mutex );

		if ( delay != 0 )
			Sleep ( delay );
	}
	while ( !salir && !p->salir );

	return 0;
}

void BattleCityServer::UpdateClients(BattleCityState state)
{
    /************************************************/
    /* Send Tanks state                           */
    /************************************************/
    for(int i = 0; i < state.Tanks.size(); i++)
    {
        vector<BattleCityTank> tanks;
        tanks.push_back(state.Tanks[i]);

        BattleCityTankPacket tankPacket(tanks);
        this->SendToAllClients(&tankPacket);
    }

    /************************************************/
    /* Send Bullets state                           */
    /************************************************/
    for(int i = 0; i < state.Bullets.size(); i++)
    {
        vector<BattleCityBullet> bullets;
        bullets.push_back(state.Bullets[i]);
        BattleCityBulletPacket bulletPacket(bullets);
        this->SendToAllClients(&bulletPacket);
    }

    /************************************************/
    /* Send Bombs state                           */
    /************************************************/
    for(int i = 0; i < state.Bombs.size(); i++)
    {
        vector<BattleCityBomb> bombs;
        bombs.push_back(state.Bombs[i]);

        BattleCityBombPacket bombPacket(bombs);
        this->SendToAllClients(&bombPacket);
    }

    /************************************************/
    /* Send Walls state                           */
    /************************************************/
    for(int i = 0; i < state.Walls.size(); i++)
    {
        vector<BattleCityWall> walls;
        walls.push_back(state.Walls[i]);
        BattleCityWallPacket wallPacket(walls);
        this->SendToAllClients(&wallPacket);
    }

    /************************************************/
    /* Send Walls state                           */
    /************************************************/
    BattleCityCommandPacket cmdPacket(UPDATESCREEN);
    this->SendToAllClients(&cmdPacket);
}

void BattleCityServer::SendToAllClients(BattleCityDataPacket * packet)
{
	for ( int i = 0 ; i < BATTLE_CITY_MAX_PLAYERS ; i++ )
    {
		if ( sockets[i] != SOCKET_ERROR )
            packet->Send(sockets[i]);
    }
}

#endif