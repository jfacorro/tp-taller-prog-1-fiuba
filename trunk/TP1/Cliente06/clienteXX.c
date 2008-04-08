// clienteXX.cpp : Defines the entry point for the console application.


#include "connector.h"
#include "transferencia.h"
#include "stdio.h"

#define MSG_SYNTAX "cliente06.exe {ip} {puerto}\n"
#define MSG_ABNORMAL	"Abnormal program termination: Error code %d\n"



int main(int argc, char* argv[])
{
	CONEXION c;
	Connector conn;
	int retval, puerto;

	if ( argc != 3 )
		printf ( MSG_SYNTAX );
	else
	{
		puerto = 0;
		sscanf ( argv[2] , "%d" , &puerto );

		if ( puerto == 0 )
			printf ( MSG_SYNTAX );
		else
		{
			printf ( "Conectando...\n" );
			retval = trConectar ( argv[1] , puerto , &c );
			if (  retval != RES_OK )
				printf ( "Abnormal program termination: Error code %d\n" , retval );
			else
			{
				printf ( "Conectado.\n" );
				InitializeConnector ( &conn , &c );
				WaitConnector ( &conn );
			}
		}
	}

	return 0;
}

