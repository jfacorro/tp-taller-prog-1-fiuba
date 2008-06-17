#ifndef BattleCityCommunicationProtocol_h
#define BattleCityCommunicationProtocol_h

#include "winsock2.h"
#include "BattleCityTypes.h"
#include "BattleCityWall.h"
#include <vector>
#include <list>

using namespace std;

#define PACKET_DATA_MAX_SIZE    4096
#define PACKET_HEADER_SIZE      3

enum BattleCityPacketType {DUMMY, TANK, BULLET, BOMB, WALL, CLRSCR, PLAYERNUMBER, COMMAND};
enum BattleCityCommandType {UPDATESCREEN};

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

        BattleCityPacketType type;

    public:
        BattleCityDataPacket()
        {
            this->buffer = NULL;
            this->type = DUMMY;
            this->size = PACKET_HEADER_SIZE;
        };

        ~BattleCityDataPacket()
        {
            if(this->buffer != NULL)
            {
                delete [] this->buffer;
                this->buffer = NULL;
            }
        }

        void SetHeaderInBuffer()
        {
            this->buffer[0] = this->type;
            this->SetSizeInBuffer();
        }

        void Send(SOCKET socket)
        {
            send(socket, this->buffer, this->size, 0);
        };

        BattleCityPacketType GetType() { return this->type; };
};

class BattleCityTankPacket : public BattleCityDataPacket
{
    public:
        BattleCityTankPacket()
        { 
            this->type = TANK; 
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
            this->size = PACKET_HEADER_SIZE;
        };

        BattleCityBulletPacket(vector<BattleCityBullet> bullets);

        BattleCityBulletPacket(char * buffer, int size);

        vector<BattleCityBullet> bullets;
};

class BattleCityBombPacket : public BattleCityDataPacket
{
    public:
        BattleCityBombPacket()
        { 
            this->type = BOMB; 
            this->size = PACKET_HEADER_SIZE;
        };

        BattleCityBombPacket(vector<BattleCityBomb> bombs);

        BattleCityBombPacket(char * buffer, int size);

        vector<BattleCityBomb> bombs;
};

class BattleCityWallPacket : public BattleCityDataPacket
{
    public:
        BattleCityWallPacket()
        { 
            this->type = WALL; 
            this->size = PACKET_HEADER_SIZE;
        };

        BattleCityWallPacket(vector<BattleCityWall> walls);

        BattleCityWallPacket(char * buffer, int size);

        vector<BattleCityWall> walls;
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
            
            this->buffer = new char[PACKET_HEADER_SIZE + sizeof(int)];
            
            this->buffer[0] = PLAYERNUMBER;
            this->size = PACKET_HEADER_SIZE;

            memcpy(this->buffer + this->size, (void*)&this->playerNumber, sizeof(int));

            this->size += sizeof(int);

            this->SetSizeInBuffer();
        };

        int GetPlayerNumber() {return this->playerNumber;};
};

class BattleCityCommandPacket : public BattleCityDataPacket
{
    private:
        BattleCityCommandType cmdType;
        
    public:
        BattleCityCommandPacket(BattleCityCommandType cmdType);
        BattleCityCommandPacket(char * buffer, int size);

        BattleCityCommandType GetCommandType() {return this->cmdType; };
};

class BattleCityCommunicationProtocol
{
    private:
        BattleCityCommunicationProtocol() {};
        static void WriteLog(const char * msg);

    public:
        static void * ReceiveDataPacket(SOCKET socket);
};

#endif