#ifndef BattleCityClient_h
#define BattleCityClient_h

#include "Socket.h"
#include "BattleCityEngine.h"
#include "SDLHelper.h"

class BattleCityClient
{
    private:
        Socket socket;
		SDLHelper sdlHelper;
        BattleCityClientParameters parameters;
        BattleCityState state;
        
        bool isConnected;
        int clientNumber;

        void UpdateEngine (int tecla);
        void RenderScreenChars();
		void RenderScreenSDL();
        
    public:
        BattleCityClient() {};

        void Connect(char * dir, int socketNumber);
        bool IsConnected() { return this->isConnected; };

        void StartPlaying();        
};

#endif