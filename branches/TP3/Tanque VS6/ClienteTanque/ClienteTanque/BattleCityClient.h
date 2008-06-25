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

        vector<Texture> textures;
        
        bool isConnected;
        int clientNumber;

        void UpdateEngine (int tecla);
        void RenderScreenChars();
		void RenderScreenSDL();
        void AddTexture(char * name, char * filename);
        SDL_Surface * GetTexture(char * name);
        
    public:
        BattleCityClient() {};

        void Connect(char * dir, int socketNumber);
        bool IsConnected() { return this->isConnected; };

        void StartPlaying();        
};

#endif