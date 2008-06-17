// ClienteTanque.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock2.h"
#include "BattleCityEngine.h"
#include "iostream"
#include "conio.h"
#include "BattleCityCommunicationProtocol.h"
#include "BattleCityClient.h"
#include "Screen.h"
#include "Socket.h"

void RenderScreen(BattleCityState& state);

/* Resulados de las funciones.*/
/* Los errores que siguen deben empezar con RES_ seguido de un nombre, por ejemplo RES_TIMEOUT */
#define RES_OK              0
#define RES_ERROR_STARTUP   1
#define RES_INVALID_SOCKET  2
#define RES_ERROR_BIND      3
#define RES_ERROR_LISTEN    4
#define RES_INACTIVE        5
#define RES_ERROR_ACCEPT    6
#define RES_ERROR_CONNECT   7
#define RES_ERROR_SEND      8
#define RES_ERROR_RECEIVE   9
#define RES_ERROR_UNKNOWN  -1

void UpdateEngine ( SOCKET s , int tecla )
{
	unsigned char buffer[6];
	buffer[0] = 2;
	buffer[1] = 0;
	buffer[2] = 6;
	buffer[3] = 0;
	buffer[4] = (tecla & 0xFF);
	buffer[5] = (tecla >> 8);

	send(s,(char*)buffer,6,0);
}


int _tmain(int argc, _TCHAR* argv[])
{
    int nroTanque = 0;

    Socket clientSock;
    int resultado = clientSock.Connect("127.0.0.1", BATTLE_CITY_SOCKET);

    if ( resultado == RES_OK )
    {
		bool salir = false;

        BattleCityClient client;

        while ( !salir )
		{
			if ( kbhit() )
			{
				int tecla = getch();
				if ( tecla == 27 ) 
					salir = true;
				else 
					UpdateEngine(clientSock.GetConnection().cxSocket, tecla);
			}

            BattleCityDataPacket * packet = (BattleCityDataPacket *)BattleCityCommunicationProtocol::ReceiveDataPacket(clientSock);

            BattleCityPlayerNumberPacket * playerNumberPacket  = NULL;
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
                        client.clientNumber = playerNumberPacket->GetPlayerNumber();
                        break;
                    case TANK:
                        tankPacket = (BattleCityTankPacket *)packet;
                        //client.state.Tanks.clear();
                        for(int i = 0; i < tankPacket->tanks.size(); i++)
                        {
                            client.state.Tanks.push_back(tankPacket->tanks[i]);
                        }
                        break;
                    case BULLET:
                        bulletPacket = (BattleCityBulletPacket *)packet;
                        //client.state.Bullets.clear();
                        for(int i = 0; i < bulletPacket->bullets.size(); i++)
                        {
                            client.state.Bullets.push_back(bulletPacket->bullets[i]);
                        }
                        break;
                    case BOMB:
                        bombPacket = (BattleCityBombPacket *)packet;
                        //client.state.Bombs.clear();
                        for(int i = 0; i < bombPacket->bombs.size(); i++)
                        {
                            client.state.Bombs.push_back(bombPacket->bombs[i]);
                        }
                        break;
                    case WALL:
                        wallPacket = (BattleCityWallPacket *)packet;
                        //client.state.Walls.clear();
                        for(int i = 0; i < wallPacket->walls.size(); i++)
                        {
                            client.state.Walls.push_back(wallPacket->walls[i]);
                        }
                        break;
                    case COMMAND:
                        cmdPacket = (BattleCityCommandPacket *)packet;
                        if(cmdPacket->GetCommandType() == UPDATESCREEN)
                        {
                            RenderScreen(client.state);

                            client.state.Tanks.clear();
                            client.state.Bullets.clear();
                            client.state.Bombs.clear();
                            client.state.Walls.clear();                                                        
                        }

                        break;
                }
            }

            delete packet;
		}
   }

	return 0;
}

void RenderScreen(BattleCityState& state)
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