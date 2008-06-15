// ClienteTanque.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock2.h"
#include "BattleCityEngine.h"
#include "iostream"
#include "conio.h"


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
		int offset = 0;
		while ( !salir )
		{
			if ( kbhit() )
			{
				int tecla = getch();
				if ( tecla == 27 ) 
					salir = true;
				else 
					UpdateEngine(sock,tecla);
			}

			int br = recv ( sock , buffer + offset , 4096 - offset , 0 );
			if ( br == -1 )
				salir = true;
			else
			{
				offset += br;
				if ( offset >= 4 )
				{
					/// Get the packet length by taking the upper 8 bits from 
                    /// buffer[3] and the lower form buffer[2].
                    int len = ((int) buffer[3] << 8) + buffer[2];

                    if ( offset >= len )
					{
						offset -= len;
						/*
						printf ( "Packet type: %X\n" , ((int) buffer[1] << 8) + buffer[0] );
						printf ( "Packet len:  %X\n" , ((int) buffer[3] << 8) + buffer[2] );
						printf ( "\n" );
						*/
						if ( buffer[0] == 0 && buffer[1] == 0 )
						{
							BattleCityTank tanks[2];
							memcpy((void*)tanks,buffer+4,2*sizeof(BattleCityTank));
							//printf ( "pos1: %f,%f  pos2: %f,%f\n" , tanks[0].Pos.X,tanks[0].Pos.Y,tanks[1].Pos.X,tanks[1].Pos.Y);
							clrscr();
							gotoxy(1,1);
							cout << "TANQUE " << nroTanque;
							gotoxy((int)tanks[0].Pos.X,(int) tanks[0].Pos.Y);
							cout << "O";
							gotoxy((int)tanks[1].Pos.X,(int) tanks[1].Pos.Y);
							cout << "O";
						}
						else if ( buffer[0] == 1 && buffer[1] == 0 )
						{
							nroTanque = buffer[4] + 1;
						}
					}
				}
			}
		}
   }

	return 0;
}

