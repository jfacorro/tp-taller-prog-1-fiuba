#ifndef BattleCityClient_h
#define BattleCityClient_h

#include "Socket.h"
#include "BattleCityEngine.h"

class BattleCityClient
{
    private:
        Socket socket;

        bool isConnected;

        void UpdateEngine (int tecla);

        void RenderScreen();
        
    public:
        BattleCityClient() {};

        void Connect(char * dir, int socketNumber);

        int clientNumber;

        BattleCityState state;

        bool IsConnected() { return this->isConnected; };

        void StartPlaying();
        
};

#endif