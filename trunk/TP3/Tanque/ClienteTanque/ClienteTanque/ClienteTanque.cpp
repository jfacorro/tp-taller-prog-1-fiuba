// ClienteTanque.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BattleCityCommunicationProtocol.h"
#include "BattleCityClient.h"
#include "SDL.h"

int _tmain(int argc, _TCHAR* argv[])
{
    int nroTanque = 0;

    BattleCityClient client;

	/// char * ip = "jfacorro.no-ip.org";
    char * ip = "127.0.0.1";
    
    client.Connect(ip, BATTLE_CITY_SOCKET);
    
    if(client.IsConnected())
    {
        client.StartPlaying();
    }
	else
	{
		printf("Not able to connect to %s", ip);
	}

    return 0;
}