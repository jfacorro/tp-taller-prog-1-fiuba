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
    
    client.Connect("190.244.19.22", BATTLE_CITY_SOCKET);
    
    if(client.IsConnected())
    {
        client.StartPlaying();
    }

    return 0;
}