#ifndef BattleCityCommunicationProtocol_h
#define BattleCityCommunicationProtocol_h

#include "winsock2.h"
#include "BattleCityTypes.h"
#include "BattleCityWall.h"
#include "BattleCityEngine.h"
#include "Socket.h"
#include <vector>
#include <list>

using namespace std;

#define PACKET_DATA_MAX_SIZE    4096
#define PACKET_HEADER_SIZE      3

#define BATTLE_CITY_SOCKET      2488

enum BattleCityPacketType {DUMMY, TANK, BULLET, BOMB, WALL, CLRSCR, PLAYERNUMBER, COMMAND, PARAMETERS};
enum BattleCityCommandType {UPDATESCREEN, KEYPRESSED};

/*****************************************************************************************/
// BattleCityDataPacket
/*****************************************************************************************/
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

/*****************************************************************************************/
// BattleCityTankPacket
/*****************************************************************************************/
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

/*****************************************************************************************/
// BattleCityBulletPacket
/*****************************************************************************************/
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

/*****************************************************************************************/
// BattleCityBombPacket
/*****************************************************************************************/
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

/*****************************************************************************************/
// BattleCityWallPacket
/*****************************************************************************************/
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

/*****************************************************************************************/
// BattleCityPlayerNumberPacket
/*****************************************************************************************/
class BattleCityPlayerNumberPacket : public BattleCityDataPacket
{
    private:
        int playerNumber;
    public:
        BattleCityPlayerNumberPacket(int playerNumber);
		BattleCityPlayerNumberPacket(char * data, int size);

        int GetPlayerNumber() {return this->playerNumber;};
};

/*****************************************************************************************/
// BattleCityParametersPacket
/*****************************************************************************************/
class BattleCityParametersPacket : public BattleCityDataPacket
{
    private:
        BattleCityClientParameters parameters;
    public:
        BattleCityParametersPacket(BattleCityClientParameters parameters);
		BattleCityParametersPacket(char * data, int size);

        BattleCityClientParameters GetParameters() { return this->parameters; };
};

/*****************************************************************************************/
// BattleCityCommandPacket
/*****************************************************************************************/
class BattleCityCommandPacket : public BattleCityDataPacket
{
    private:
        BattleCityCommandType cmdType;
        int auxValue;
        int clientNumber;

        void CstorHelper();
        
    public:
        BattleCityCommandPacket(BattleCityCommandType cmdType);
        BattleCityCommandPacket(BattleCityCommandType cmdType, int clientNumber, int auxValue);
        BattleCityCommandPacket(char * data, int size);

        BattleCityCommandType GetCommandType() {return this->cmdType; };

        bool FromClient() { return (this->clientNumber != -1); };

        int GetClientNumber() { return this->clientNumber; };
        int GetAuxValue() { return this->auxValue; };
};

/*****************************************************************************************/
// BattleCityCommunicationProtocol
/*****************************************************************************************/
class BattleCityCommunicationProtocol
{
    private:
        BattleCityCommunicationProtocol() {};
        static void WriteLog(const char * msg);

    public:
        static void * ReceiveDataPacket(Socket socket);
        static void * ReceiveDataPacket(SOCKET socket);
};

#endif