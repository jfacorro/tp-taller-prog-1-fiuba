// servidorXX.cpp : Defines the entry point for the console application.

#include "stdio.h"
#include "parser.h"
#include "conio.h"
#include "connector.h"
#include "windows.h"
#include "transferencia.h"


#define MSG_SYNTAX		"servidor06.exe {puerto}\n"
#define MSG_ABNORMAL	"Abnormal program termination: Error code %d\n"

int main(int argc, char* argv[])
{
	CONEXION c;
	Connector conn;
	int retval, puerto;

	if ( argc != 2 )
		printf ( MSG_SYNTAX );
	else
	{
		puerto = 0;
		sscanf ( argv[1] , "%d" , &puerto );
		if ( puerto == 0 )
			printf ( MSG_SYNTAX );
		else
		{
			printf ( "Escuchando...\n" );
			retval = trEscuchar ( puerto , &c );
			if (  retval != RES_OK )
				printf ( "Abnormal program termination: Error code %d\n" , retval );
			else
			{
				printf ( "Conexion recibida.\n" );
				InitializeConnector ( &conn , &c );
				WaitConnector ( &conn );
			}
		}
	}

	return 0;
}

