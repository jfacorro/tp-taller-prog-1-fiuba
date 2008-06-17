// ClienteTanque.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock2.h"
#include "BattleCityEngine.h"
#include "iostream"
#include "conio.h"
#include "BattleCityCommunicationProtocol.h"
#include "BattleCityClient.h"
#include "Screen.h"
#include "Socket.h"

int _tmain(int argc, _TCHAR* argv[])
{
    int nroTanque = 0;

    BattleCityClient client;
    
    client.Connect("127.0.0.1", BATTLE_CITY_SOCKET);
    
    if(client.IsConnected())
    {
        client.StartPlaying();
    }

    return 0;
}