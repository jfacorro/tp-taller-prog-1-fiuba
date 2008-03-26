#ifndef __CONNECTOR_INCLUDED
#define __CONNECTOR_INCLUDED

#include "transferencias.h"
#include "windows.h"


typedef void (*CONNECTOR_DATA_CALLBACK) (char*,int);
typedef void (*CONNECTOR_ERROR_CALLBACK) (int);

typedef struct _Connector
{
	CONEXION*					pConexion;
	CONNECTOR_DATA_CALLBACK		dataCallback;
	CONNECTOR_ERROR_CALLBACK	errorCallback;
	HANDLE						hThrReceive;
	HANDLE						hThrSend;
	BOOL						salir;
}
	Connector;


void InitializeConnector ( Connector* connector );
void FinalizeConnector   ( Connector* connector );

#endif
