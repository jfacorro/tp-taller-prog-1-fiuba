// servidorXX.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "parser.h"
#include "conio.h"
#include "connector.h"
#include "windows.h"
#include "transferencia.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CONEXION c;
	Connector conn;
	
	printf ( "Escuchando...\n" );
	trEscuchar ( 6800 , &c );
	printf ( "Conexion recibida.\n" );

	InitializeConnector ( &conn , &c );
	WaitConnector ( &conn );
	return 0;
}

