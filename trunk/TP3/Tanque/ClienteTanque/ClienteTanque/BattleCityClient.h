#ifndef BattleCityClient_h
#define BattleCityClient_h

#include "Socket.h"
#include "BattleCityCommunicationProtocol.h"
#include "BattleCityEngine.h"
#include "SDLHelper.h"
#include "SDL_ttf.h"
#include "map"

using namespace std;

#define MAX_TANKS		2
#define MAX_DIRECTIONS	5

class BattleCityClient
{
    private:
        Socket socket;
		SDLHelper sdlHelper;
        TTF_Font * sdlFont;
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

        void ReceiveParametersAndTextures();

        static DWORD __stdcall ReceiveDataFromServer ( LPVOID param );
        static void ProcessPacket(BattleCityClient * client, BattleCityDataPacket * packet);

        Rect GetScaledRectangle(Rect source, Rect quadrant, int pixelsPerUM);

        HANDLE mutex;

	    SDL_Surface *	background;
		SDL_Surface*	tanks[MAX_TANKS][MAX_DIRECTIONS];
		map<int,SDL_Surface*>	walls;

    public:
		BattleCityClient() : background(NULL)
        {
            this->mutex = CreateMutex ( NULL , FALSE , NULL );

			for ( int i = 0 ; i < MAX_TANKS ; i++ )
				for ( int j = 0 ; j < MAX_DIRECTIONS ; j++ )
					tanks[i][j] = NULL;
        };

        void Connect(char * dir, int socketNumber);
        bool IsConnected() { return this->isConnected; };

        void StartPlaying();        
};

#endif