#ifndef BattleCityClient_h
#define BattleCityClient_h

#include "Socket.h"
#include "BattleCityCommunicationProtocol.h"
#include "BattleCityEngine.h"
#include "SDLHelper.h"

class BattleCityClient
{
    private:
        Socket socket;
		SDLHelper sdlHelper;
        BattleCityClientParameters parameters;
        BattleCityState state;

        vector<Texture *> textures;

		int nextTick;
		int lastTick;

		void InitializeTicks();
		void UpdateTicks();
		int GetDelayTime() { return nextTick - lastTick; };
        
        bool isConnected;
        int clientNumber;

        void UpdateEngine (int tecla);
        void RenderScreenChars();
		void RenderScreenSDL();
        void AddTexture(char * name, char * filename);
        SDL_Surface * GetTexture(char * name);

        void ReceiveParameters();

        static DWORD __stdcall ReceiveDataFromServer ( LPVOID param );
        static void ProcessPacket(BattleCityClient * client, BattleCityDataPacket * packet);

        Rect GetScaledRectangle(Rect source, Rect quadrant, int pixelsPerUM);

        HANDLE mutex;

    public:
        BattleCityClient() 
        {
            this->mutex = CreateMutex ( NULL , FALSE , NULL );
        };

        void Connect(char * dir, int socketNumber);
        bool IsConnected() { return this->isConnected; };

        void StartPlaying();        
};

#endif