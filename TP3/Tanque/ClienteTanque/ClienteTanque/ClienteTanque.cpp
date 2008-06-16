// ClienteTanque.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock2.h"
#include "BattleCityEngine.h"
#include "iostream"
#include "conio.h"
#include "BattleCityCommunicationProtocol.h"
#include "BattleCityClient.h"

void clrscr()
{
	COORD c;
	c.X = 0;
	c.Y = 0;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE),' ',25 * 80 , c , NULL );
}

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

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

   WSADATA wsaData; /* Utilizada para inicializacion del winsock */
   WORD wVersionReq = MAKEWORD(2, 2); /* Contiene la version */
   SOCKET sock; /* Contiene la informacion del socket */
   //SOCKET tempSock; /* Se utiliza para no sobreescribir informacion al aceptar la conexion */   
   struct hostent* hostInfo; /* Se utiliza para convertir el nombre del host a su direccion IP */
   struct sockaddr_in sockAddrIn; /* Direccion de socket */
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
         sockAddrIn.sin_family = AF_INET;       /* Protocolo ipv4 */
         sockAddrIn.sin_port = htons(2488);   /* Puerto indicado */
         hostInfo = gethostbyname("127.0.0.1");  /* Direccion IP */
         sockAddrIn.sin_addr = *((struct in_addr *)((*hostInfo).h_addr));
         
            codError = connect(sock, (SOCKADDR *)(&sockAddrIn), sizeof(sockAddrIn));
            if (codError != 0) {
               resultado = RES_ERROR_CONNECT;
            } else {
               resultado = RES_OK;
            }
	      }
   }
	
   if ( resultado == RES_OK )
   {
		char buffer [ 4096 ];
		bool salir = false;
		int writeFromOffset = 0;
        int readFromOffset = 0;

        BattleCityClient client;

        while ( !salir )
		{
			if ( kbhit() )
			{
				int tecla = getch();
				if ( tecla == 27 ) 
					salir = true;
				else 
					UpdateEngine(sock, tecla);
			}

            BattleCityDataPacket * packet = (BattleCityDataPacket *)BattleCityCommunicationProtocol::ReceiveDataPacket(sock);

            BattleCityPlayerNumberPacket * playerNumberPacket  = NULL;
            BattleCityTankPacket * tankPacket = NULL;
            BattleCityBulletPacket * bulletPacket = NULL;

            if(packet != NULL)
            {
                switch(packet->GetType())
                {
                    case DUMMY:
                        break;
                    case PLAYERNUMBER:
                        playerNumberPacket = (BattleCityPlayerNumberPacket *)packet;

                        break;
                    case TANK:
                        tankPacket = (BattleCityTankPacket *)packet;
                        client.state.Tanks.clear();
                        for(int i = 0; i < tankPacket->tanks.size(); i++)
                        {
                            client.state.Tanks.push_back(tankPacket->tanks[i]);
                        }
                        break;
                    case BULLET:
                        bulletPacket = (BattleCityBulletPacket *)packet;
                        client.state.Bullets.clear();
                        for(int i = 0; i < bulletPacket->bullets.size(); i++)
                        {
                            client.state.Bullets.push_back(bulletPacket->bullets[i]);
                        }
                        break;
                }
            }

            delete packet;

            RenderScreen(client.state);
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

    list<BattleCityBomb>::iterator iter = state.Bombs.begin();
	for ( ; iter != state.Bombs.end() ; ++iter )
	{
		if ( iter->TimeToDie >= 0 )
		{
			gotoxy(iter->Pos.X,iter->Pos.Y);
			cout << "B";
		}
		else
		{
			gotoxy(iter->Pos.X,iter->Pos.Y-1);
			cout << "*";
			gotoxy(iter->Pos.X-1,iter->Pos.Y);
			cout << "***";
			gotoxy(iter->Pos.X,iter->Pos.Y+1);
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