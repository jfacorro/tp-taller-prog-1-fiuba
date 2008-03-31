// clienteXX.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "connector.h"
#include "transferencia.h"
#include "stdio.h"
#include "conio.h"

int main(int argc, CHAR* argv[])
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


