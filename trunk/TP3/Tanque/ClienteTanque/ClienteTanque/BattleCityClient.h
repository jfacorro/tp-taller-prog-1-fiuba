#ifndef BattleCityClient_h
#define BattleCityClient_h

#include "Socket.h"
#include "BattleCityEngine.h"

class BattleCityClient
{
    private:
        Socket socket;
        
    public:
        BattleCityClient() {};

        int clientNumber;

        BattleCityState state;
};

#endif