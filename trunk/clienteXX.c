// clienteXX.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../servidorXX/connector.h"
#include "../servidorXX/transferencia.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CONEXION c;
	Connector conn;

	printf ( "Conectando...\n" );
	trConectar ( "127.0.0.1" , 6800 , &c );
	printf ( "Conectado.\n" );

	InitializeConnector ( &conn , &c );
	WaitConnector ( &conn );
	return 0;
}

