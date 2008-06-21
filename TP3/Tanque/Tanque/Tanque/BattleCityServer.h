#ifndef BATTLECITYSERVER_INCLUDED
#define BATTLECITYSERVER_INCLUDED

#include "winsock2.h"

#include "BattleCityEngine.h"
#include "BattleCityCommunicationProtocol.h"

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

#define BATTLE_CITY_MAX_PLAYERS	2

class BattleCityServer
{
public:
	BattleCityServer(int port,BattleCityParameters parameters);
	virtual ~BattleCityServer();

	int Start();
	void Stop();

private:
	static DWORD __stdcall ThreadProc ( LPVOID param );
	static DWORD __stdcall MainThread ( LPVOID param );

	void NewConnection ( SOCKET s );
	void UpdateClients ( BattleCityState state );
    void SendToAllClients ( char * buffer, int size );
    void SendToAllClients(BattleCityDataPacket * packet);
    void BattleCityServer::SendToClient(BattleCityDataPacket * packet, SOCKET sock);

	void OnKey ( int tank , int key );

	BattleCityParameters parameters;
	BattleCityEngine* engine;

	int port;
	bool salir;

	SOCKET sock; 
	SOCKET sockets[BATTLE_CITY_MAX_PLAYERS];

	HANDLE mutex;
};

#endif
