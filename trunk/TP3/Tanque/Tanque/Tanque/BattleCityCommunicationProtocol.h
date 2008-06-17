#ifndef BattleCityCommunicationProtocol_h
#define BattleCityCommunicationProtocol_h

#include "winsock2.h"
#include "BattleCityTypes.h"
#include "BattleCityWall.h"
#include "Socket.h"
#include <vector>
#include <list>

using namespace std;

#define PACKET_DATA_MAX_SIZE    4096
#define PACKET_HEADER_SIZE      3

#define BATTLE_CITY_SOCKET      2488

enum BattleCityPacketType {DUMMY, TANK, BULLET, BOMB, WALL, CLRSCR, PLAYERNUMBER, COMMAND};
enum BattleCityCommandType {UPDATESCREEN};

class BattleCityDataPacket : public SocketPacket
{ 
    protected:
        void SetSizeInData()
        {
            this->data[1] = (size & 0xFF);
            this->data[2] = (size >> 8);
        };

        BattleCityPacketType type;

    public:
        BattleCityDataPacket()
        {
            this->data = NULL;
            this->type = DUMMY;
            this->size = PACKET_HEADER_SIZE;
        };

        void SetHeaderInData()
        {
            this->data[0] = this->type;
            this->SetSizeInData();
        }

        void Send(SOCKET socket)
        {
            send(socket, this->data, this->size, 0);
        };

        static int GetSizeFromData(char * data)
        {
            return ((int) data[2] << 8) + data[1];
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

        BattleCityTankPacket(char * data, int size);

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

        BattleCityBulletPacket(char * data, int size);

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

        BattleCityBombPacket(char * data, int size);

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

        BattleCityWallPacket(char * data, int size);

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
            
            this->data = new char[PACKET_HEADER_SIZE + sizeof(int)];
            
            this->data[0] = PLAYERNUMBER;
            this->size = PACKET_HEADER_SIZE;

            memcpy(this->data + this->size, (void*)&this->playerNumber, sizeof(int));

            this->size += sizeof(int);

            this->SetSizeInData();
        };

        int GetPlayerNumber() {return this->playerNumber;};
};

class BattleCityCommandPacket : public BattleCityDataPacket
{
    private:
        BattleCityCommandType cmdType;
        
    public:
        BattleCityCommandPacket(BattleCityCommandType cmdType);
        BattleCityCommandPacket(char * data, int size);

        BattleCityCommandType GetCommandType() {return this->cmdType; };
};

class BattleCityCommunicationProtocol
{
    private:
        BattleCityCommunicationProtocol() {};
        static void WriteLog(const char * msg);

    public:
        static void * ReceiveDataPacket(Socket socket);
};

#endif