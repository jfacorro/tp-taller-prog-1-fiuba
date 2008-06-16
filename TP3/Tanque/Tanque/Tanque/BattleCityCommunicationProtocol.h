#ifndef BattleCityCommunicationProtocol_h
#define BattleCityCommunicationProtocol_h

#include "winsock2.h"
#include "BattleCityTypes.h"
#include <vector>

using namespace std;

#define PACKET_DATA_MAX_SIZE    4096
#define PACKET_HEADER_SIZE      3

enum BattleCityPacketType {DUMMY, TANK, BULLET, BOMB, WALL, CLRSCR, PLAYERNUMBER};

class BattleCityDataPacket
{ 
    protected:
        char * buffer;
        int size;

        void SetSizeInBuffer()
        {
            this->buffer[1] = (size & 0xFF);
            this->buffer[2] = (size >> 8);
        };

        int GetSizeFromBuffer()
        {
            return ((int) buffer[2] << 8) + buffer[1];
        };

    public:
        BattleCityDataPacket()
        {
            this->type = DUMMY;
            this->size = PACKET_HEADER_SIZE;

            this->SetSizeInBuffer();
        };

        void SetHeaderInBuffer()
        {
            this->buffer[0] = this->type;
        }

        void Send(SOCKET socket)
        {
            send(socket, this->buffer, this->size, 0);
        };

        BattleCityPacketType GetType() { return this->type; };

        BattleCityPacketType type;
};

class BattleCityTankPacket : public BattleCityDataPacket
{
    public:
        BattleCityTankPacket()
        { 
            this->type = TANK; 
            this->buffer[0] = TANK;
            this->size = PACKET_HEADER_SIZE;
        };
        BattleCityTankPacket(vector<BattleCityTank> tanks);
        BattleCityTankPacket(char * buffer, int size);

        vector<BattleCityTank> tanks;
};

class BattleCityBulletPacket : public BattleCityDataPacket
{
    public:
        BattleCityBulletPacket()
        { 
            this->type = BULLET; 
            this->buffer[0] = BULLET;
            this->size = PACKET_HEADER_SIZE;
        };
        BattleCityBulletPacket(vector<BattleCityBullet> bullets);
        BattleCityBulletPacket(char * buffer, int size);

        vector<BattleCityBullet> bullets;
};

class BattleCityPlayerNumberPacket : public BattleCityDataPacket
{
    private:
        int playerNumber;
    public:
        BattleCityPlayerNumberPacket(int playerNumber)
        { 
            this->playerNumber = playerNumber;
            this->type = PLAYERNUMBER; 
            this->buffer[0] = PLAYERNUMBER;
            this->size = PACKET_HEADER_SIZE;

            memcpy(this->buffer + this->size, (void*)&this->playerNumber, sizeof(int));

            this->size += sizeof(int);

            this->SetSizeInBuffer();
        };
};

class BattleCityCommunicationProtocol
{
    private:
        BattleCityCommunicationProtocol() {};

    public:
        static void * ReceiveDataPacket(SOCKET socket);
};

#endif