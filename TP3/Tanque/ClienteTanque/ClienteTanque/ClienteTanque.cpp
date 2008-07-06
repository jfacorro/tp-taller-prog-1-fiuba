// ClienteTanque.cpp : Defines the entry point for the console application.
//

#include "BattleCityCommunicationProtocol.h"
#include "BattleCityClient.h"
#include "SDL.h"

int main(int argc, char* argv[])
{
    int nroTanque = 0;

    BattleCityClient client;

	/// char * ip = "jfacorro.no-ip.org";
    char ip[50]; 
    printf("Please enter the server's ip or hostname: ");
    scanf("%s", ip);
    
    client.Connect(ip, BATTLE_CITY_SOCKET);

    if(client.IsConnected())
    {
        printf("Connect to %s\n", ip);
        client.StartPlaying();
    }
	else
	{
		printf("Not able to connect to %s", ip);
	}

    return 0;
}