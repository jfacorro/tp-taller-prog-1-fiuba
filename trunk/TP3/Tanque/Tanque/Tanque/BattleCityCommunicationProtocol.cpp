#ifndef BattleCityCommunicationProtocol_cpp
#define BattleCityCommunicationProtocol_cpp

#include "BattleCityCommunicationProtocol.h"
#include "Screen.h"

/**********************************************************************************************************/

BattleCityTankPacket::BattleCityTankPacket(vector<BattleCityTank> tanks)
{
    this->type = TANK; 
    this->size = PACKET_HEADER_SIZE;

    this->tanks = tanks;

    int totalPacketSize = this->size + sizeof(BattleCityTank) * tanks.size();
    this->buffer = new char[totalPacketSize];

    for(int i = 0; i < tanks.size(); i++)
    {
        memcpy(this->buffer + this->size, (void*)&this->tanks[i], sizeof(BattleCityTank));
        this->size += sizeof(BattleCityTank);
    }
    
    this->SetHeaderInBuffer();
};

BattleCityTankPacket::BattleCityTankPacket(char * buffer, int size)
{
    this->type = TANK;
    this->size = PACKET_HEADER_SIZE;

    //this->buffer = new char[size];

    //memcpy((void *)this->buffer, buffer, size);

    this->size = size;
    int numberOfTanks = (this->size - PACKET_HEADER_SIZE) / sizeof(BattleCityTank);

    unsigned long offset = (unsigned long)buffer + PACKET_HEADER_SIZE;

    for(int i = 0; i < numberOfTanks; i++)
    {
        offset = offset + sizeof(BattleCityTank) * i;
        BattleCityTank tank[1];
        memcpy((void*)tank, (void *)(offset), sizeof(BattleCityTank));
        this->tanks.push_back(tank[0]);
    }
}
/**********************************************************************************************************/
// BattleCityBulletPacket
/**********************************************************************************************************/

BattleCityBulletPacket::BattleCityBulletPacket(vector<BattleCityBullet> bullets)
{
    this->type = BULLET; 
    this->size = PACKET_HEADER_SIZE;

    this->bullets = bullets;

    int totalPacketSize = this->size + sizeof(BattleCityBullet) * bullets.size();
    this->buffer = new char[totalPacketSize];

    for(int i = 0; i < bullets.size(); i++)
    {
        memcpy(this->buffer + this->size, (void*)&this->bullets[i], sizeof(BattleCityBullet));
        this->size += sizeof(BattleCityBullet);                
    }
    
    this->SetHeaderInBuffer();
};

BattleCityBulletPacket::BattleCityBulletPacket(char * buffer, int size)
{
    this->type = BULLET; 
    this->size = PACKET_HEADER_SIZE;

    this->size = size;
    int numberOfBullets = (this->size - PACKET_HEADER_SIZE) / sizeof(BattleCityBullet);

    unsigned long offset = (unsigned long)buffer + PACKET_HEADER_SIZE;

    for(int i = 0; i < numberOfBullets; i++)
    {
        offset = offset + sizeof(BattleCityBullet) * i;
        BattleCityBullet bullet;
        memcpy((void*)(&bullet), (void *)(offset), sizeof(BattleCityBullet));
        this->bullets.push_back(bullet);
    }
}

/**********************************************************************************************************/
// BattleCityBombPacket
/**********************************************************************************************************/

BattleCityBombPacket::BattleCityBombPacket(vector<BattleCityBomb> bombs)
{
    this->type = BOMB; 
    this->size = PACKET_HEADER_SIZE;

    this->bombs = bombs;

    int totalPacketSize = this->size + sizeof(BattleCityBomb) * bombs.size();
    this->buffer = new char[totalPacketSize];

    for(int i = 0; i < bombs.size(); i++)
    {
        memcpy(this->buffer + this->size, (void*)&this->bombs[i], sizeof(BattleCityBomb));
        this->size += sizeof(BattleCityBomb);                
    }
    
    this->SetHeaderInBuffer();
};

BattleCityBombPacket::BattleCityBombPacket(char * buffer, int size)
{
    this->type = BOMB; 
    this->size = PACKET_HEADER_SIZE;

    this->size = size;
    int numberOfBombs = (this->size - PACKET_HEADER_SIZE) / sizeof(BattleCityBomb);

    unsigned long offset = (unsigned long)buffer + PACKET_HEADER_SIZE;

    for(int i = 0; i < numberOfBombs; i++)
    {
        offset = offset + sizeof(BattleCityBomb) * i;
        BattleCityBomb bomb;
        memcpy((void*)(&bomb), (void *)(offset), sizeof(BattleCityBomb));
        this->bombs.push_back(bomb);
    }
}

/**********************************************************************************************************/
// BattleCityWallPacket
/**********************************************************************************************************/

BattleCityWallPacket::BattleCityWallPacket(vector<BattleCityWall> walls)
{
    this->type = WALL; 
    this->size = PACKET_HEADER_SIZE;

    this->walls = walls;

    int totalPacketSize = this->size + sizeof(BattleCityWall) * walls.size();
    this->buffer = new char[totalPacketSize];

    for(int i = 0; i < walls.size(); i++)
    {
        memcpy(this->buffer + this->size, (void*)&this->walls[i], sizeof(BattleCityWall));
        this->size += sizeof(BattleCityWall);                
    }
    
    this->SetHeaderInBuffer();
};

BattleCityWallPacket::BattleCityWallPacket(char * buffer, int size)
{
    this->type = WALL; 
    this->size = PACKET_HEADER_SIZE;

    this->size = size;
    int numberOfWalls = (this->size - PACKET_HEADER_SIZE) / sizeof(BattleCityWall);

    unsigned long offset = (unsigned long)buffer + PACKET_HEADER_SIZE;

    for(int i = 0; i < numberOfWalls; i++)
    {
        offset = offset + sizeof(BattleCityWall) * i;
        BattleCityWall wall;
        memcpy((void*)(&wall), (void *)(offset), sizeof(BattleCityWall));
        this->walls.push_back(wall);
    }
}

/**********************************************************************************************************/
// BattleCityWallPacket
/**********************************************************************************************************/

BattleCityCommandPacket::BattleCityCommandPacket(BattleCityCommandType cmdType)
{ 
    this->cmdType = cmdType;
    this->type = COMMAND; 
    
    this->buffer = new char[PACKET_HEADER_SIZE + sizeof(BattleCityCommandType)];
    
    this->buffer[0] = COMMAND;
    this->size = PACKET_HEADER_SIZE;

    memcpy(this->buffer + this->size, (void*)&this->cmdType, sizeof(BattleCityCommandType));

    this->size += sizeof(BattleCityCommandType);

    this->SetSizeInBuffer();
};

BattleCityCommandPacket::BattleCityCommandPacket(char * buffer, int size)
{
    this->type = COMMAND;
    this->size = size;
    
    int offset = (int)buffer + PACKET_HEADER_SIZE;

    memcpy((void*)&(this->cmdType), (void * )offset, sizeof(BattleCityCommandType));
}

/**********************************************************************************************************/

void * BattleCityCommunicationProtocol::ReceiveDataPacket(SOCKET socket)
{
    BattleCityDataPacket * packet = NULL;

    char packetHeader[PACKET_HEADER_SIZE];

    /// Get Packet Header
    int bytesRead = recv ( socket , packetHeader, 3 , 0 );

    if(bytesRead != -1)
    {
        if ( bytesRead >= 3 )
        {
            int packetTotalLength = ((int) packetHeader[2] << 8) + packetHeader[1];
            if(packetTotalLength > 0)
            {
                int packetDataLength = packetTotalLength - PACKET_HEADER_SIZE;

                WriteLog("char * packetData = new char[packetTotalLength];\n");
                char number[10];
                itoa(packetTotalLength, number, 10);
                WriteLog(number);
                WriteLog("\n");

                char * packetData = new char[packetTotalLength];
                if(packetData != 0)
                {
                    packetData[0] = packetHeader[0];
                    packetData[1] = packetHeader[1];
                    packetData[2] = packetHeader[2];

                    int tmp;
                    int cantRecibida = 0, totalRecibido = 0;

                    bool successTransfer = true;

                    while ( totalRecibido < packetDataLength )
                    {
                        cantRecibida = recv (socket, packetData + PACKET_HEADER_SIZE + totalRecibido, packetDataLength - totalRecibido, 0);

                        if ( cantRecibida == 0 || cantRecibida == -1 )
                            successTransfer = false;

                        totalRecibido += cantRecibida;
                        if (totalRecibido >= packetDataLength) 
                            break;
                    }

                    if(successTransfer)
                    {
                        if ( packetData[0] == TANK)
		                {
                            WriteLog("BattleCityTankPacket * tankPacket = new BattleCityTankPacket(packetData, packetTotalLength);\n");

                            BattleCityTankPacket * tankPacket = new BattleCityTankPacket(packetData, packetTotalLength);
                            packet = tankPacket;
		                }

                        if ( packetData[0] == BULLET)
		                {
                            WriteLog("BattleCityBulletPacket * bulletPacket = new BattleCityBulletPacket(packetData, packetTotalLength);\n");

                            BattleCityBulletPacket * bulletPacket = new BattleCityBulletPacket(packetData, packetTotalLength);
                            packet = bulletPacket;
		                }

                        if ( packetData[0] == BOMB)
		                {
                            WriteLog("BattleCityBombPacket * bombPacket = new BattleCityBombPacket(packetData, packetTotalLength);\n");

                            BattleCityBombPacket * bombPacket = new BattleCityBombPacket(packetData, packetTotalLength);
                            packet = bombPacket;
		                }

                        if ( packetData[0] == WALL)
		                {
                            WriteLog("BattleCityWall * wallPacket = new BattleCityWallPacket(packetData, packetTotalLength);\n");

                            BattleCityWallPacket * wallPacket = new BattleCityWallPacket(packetData, packetTotalLength);
                            packet = wallPacket;
		                }


                        if ( packetData[0] == COMMAND)
		                {
                            WriteLog("BattleCityCommandPacket * cmdPacket = new BattleCityCommandPacket(packetData, packetTotalLength);");

                            BattleCityCommandPacket * cmdPacket = new BattleCityCommandPacket(packetData, packetTotalLength);
                            packet = cmdPacket;
		                }
                    }

                    delete packetData;
                }
            }
        }
    }

    return packet;
}

void BattleCityCommunicationProtocol::WriteLog(const char * msg)
{
    /*    
    FILE * file;

    file = fopen("log.txt", "a");

    if (file!=NULL)
    {
        fputs (msg, file);
        fclose (file);
    }
    */
}

#endif
