#ifndef BattleCityCommunicationProtocol_cpp
#define BattleCityCommunicationProtocol_cpp

#include "BattleCityCommunicationProtocol.h"
#include "Screen.h"

/**********************************************************************************************************/
// BattleCityTankPacket
/**********************************************************************************************************/
BattleCityTankPacket::BattleCityTankPacket(vector<BattleCityTank> tanks)
{
    this->type = TANK; 
    this->size = PACKET_HEADER_SIZE;

    this->tanks = tanks;

    int totalPacketSize = this->size + sizeof(BattleCityTank) * tanks.size();
    this->data = new char[totalPacketSize];

    for(int i = 0; i < tanks.size(); i++)
    {
        memcpy(this->data + this->size, (void*)&this->tanks[i], sizeof(BattleCityTank));
        this->size += sizeof(BattleCityTank);
    }
    
    this->SetHeaderInData();
};

BattleCityTankPacket::BattleCityTankPacket(char * data, int size)
{
    this->type = TANK;
    this->size = PACKET_HEADER_SIZE;

    //this->data = new char[size];

    //memcpy((void *)this->data, data, size);

    this->size = size;
    int numberOfTanks = (this->size - PACKET_HEADER_SIZE) / sizeof(BattleCityTank);

    unsigned long offset = (unsigned long)data + PACKET_HEADER_SIZE;

    for(int i = 0; i < numberOfTanks; i++)
    {
        offset = offset + sizeof(BattleCityTank) * i;
        BattleCityTank tank(1);
        memcpy((void*)&tank, (void *)(offset), sizeof(BattleCityTank));
        this->tanks.push_back(tank);
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
    this->data = new char[totalPacketSize];

    for(int i = 0; i < bullets.size(); i++)
    {
        memcpy(this->data + this->size, (void*)&this->bullets[i], sizeof(BattleCityBullet));
        this->size += sizeof(BattleCityBullet);                
    }
    
    this->SetHeaderInData();
};

BattleCityBulletPacket::BattleCityBulletPacket(char * data, int size)
{
    this->type = BULLET; 
    this->size = PACKET_HEADER_SIZE;

    this->size = size;
    int numberOfBullets = (this->size - PACKET_HEADER_SIZE) / sizeof(BattleCityBullet);

    unsigned long offset = (unsigned long)data + PACKET_HEADER_SIZE;

    for(int i = 0; i < numberOfBullets; i++)
    {
        offset = offset + sizeof(BattleCityBullet) * i;
        BattleCityBullet bullet(1);
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
    this->data = new char[totalPacketSize];

    for(int i = 0; i < bombs.size(); i++)
    {
        memcpy(this->data + this->size, (void*)&this->bombs[i], sizeof(BattleCityBomb));
        this->size += sizeof(BattleCityBomb);                
    }
    
    this->SetHeaderInData();
};

BattleCityBombPacket::BattleCityBombPacket(char * data, int size)
{
    this->type = BOMB; 
    this->size = PACKET_HEADER_SIZE;

    this->size = size;
    int numberOfBombs = (this->size - PACKET_HEADER_SIZE) / sizeof(BattleCityBomb);

    unsigned long offset = (unsigned long)data + PACKET_HEADER_SIZE;

    for(int i = 0; i < numberOfBombs; i++)
    {
        offset = offset + sizeof(BattleCityBomb) * i;
        BattleCityBomb bomb(1);
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
    this->data = new char[totalPacketSize];

    for(int i = 0; i < walls.size(); i++)
    {
        memcpy(this->data + this->size, (void*)&this->walls[i], sizeof(BattleCityWall));
        this->size += sizeof(BattleCityWall);                
    }
    
    this->SetHeaderInData();
};

BattleCityWallPacket::BattleCityWallPacket(char * data, int size)
{
    this->type = WALL; 
    this->size = PACKET_HEADER_SIZE;

    this->size = size;
    int numberOfWalls = (this->size - PACKET_HEADER_SIZE) / sizeof(BattleCityWall);

    unsigned long offset = (unsigned long)data + PACKET_HEADER_SIZE;

    for(int i = 0; i < numberOfWalls; i++)
    {
        offset = offset + sizeof(BattleCityWall) * i;
        BattleCityWall wall;
        memcpy((void*)(&wall), (void *)(offset), sizeof(BattleCityWall));
        this->walls.push_back(wall);
    }
}

/**********************************************************************************************************/
// BattleCityCommandPacket
/**********************************************************************************************************/
void BattleCityCommandPacket::CstorHelper()
{
    this->type = COMMAND; 

    int fullSize = PACKET_HEADER_SIZE + sizeof(BattleCityCommandType) + 2 * sizeof(int);
    
    this->data = new char[fullSize];
    
    this->data[0] = COMMAND;
    this->size = PACKET_HEADER_SIZE;

    memcpy(this->data + this->size, (void*)&this->cmdType, sizeof(BattleCityCommandType));
    this->size += sizeof(BattleCityCommandType);
    memcpy(this->data + this->size, (void*)&this->clientNumber, sizeof(int));
    this->size += sizeof(int);
    memcpy(this->data + this->size, (void*)&this->auxValue, sizeof(int));
    this->size += sizeof(int);

    this->SetSizeInData();
}

BattleCityCommandPacket::BattleCityCommandPacket(BattleCityCommandType cmdType, int clientNumber, int auxValue) : clientNumber(clientNumber), cmdType (cmdType), auxValue(auxValue)
{ 
    this->CstorHelper();
};

BattleCityCommandPacket::BattleCityCommandPacket(BattleCityCommandType cmdType) : cmdType (cmdType)
{ 
    this->CstorHelper();
};

BattleCityCommandPacket::BattleCityCommandPacket(char * data, int size)
{
    this->type = COMMAND;
    this->size = size;
    
    int offset = (int)data + PACKET_HEADER_SIZE;

    memcpy((void*)&(this->cmdType), (void * )offset, sizeof(BattleCityCommandType));
    offset += sizeof(BattleCityCommandType);
    memcpy((void*)&(this->clientNumber), (void * )offset, sizeof(int));
    offset += sizeof(int);
    memcpy((void*)&(this->auxValue), (void * )offset, sizeof(int));
    offset += sizeof(int);
}

/**********************************************************************************************************/
// BattleCityPlayerNumberPacket
/**********************************************************************************************************/
BattleCityPlayerNumberPacket::BattleCityPlayerNumberPacket(int playerNumber)
{ 
    this->playerNumber = playerNumber;
    this->type = PLAYERNUMBER; 
    
    this->data = new char[PACKET_HEADER_SIZE + sizeof(int)];
    
    this->data[0] = PLAYERNUMBER;
    this->size = PACKET_HEADER_SIZE;

    memcpy(this->data + this->size, (void*)&this->playerNumber, sizeof(int));

    this->size += sizeof(int);

    this->SetSizeInData();
}

BattleCityPlayerNumberPacket::BattleCityPlayerNumberPacket(char * data, int size)
{
	this->type = PLAYERNUMBER;
	this->size = size;

	int offset = (int)data + PACKET_HEADER_SIZE;

	memcpy((void*)&(this->playerNumber), (void * )offset, sizeof(int));
}

/**********************************************************************************************************/
// BattleCityParametersPacket
/**********************************************************************************************************/
BattleCityParametersPacket::BattleCityParametersPacket(BattleCityClientParameters parameters)
{ 
    this->parameters = parameters;
    this->type = PARAMETERS; 
    
    this->data = new char[PACKET_HEADER_SIZE + sizeof(BattleCityClientParameters)];
    
    this->data[0] = PARAMETERS;
    this->size = PACKET_HEADER_SIZE;

    memcpy(this->data + this->size, (void*)&this->parameters, sizeof(BattleCityClientParameters));

    this->size += sizeof(BattleCityClientParameters);

    this->SetSizeInData();
}

BattleCityParametersPacket::BattleCityParametersPacket(char * data, int size)
{
	this->type = PARAMETERS;
	this->size = size;

	int offset = (int)data + PACKET_HEADER_SIZE;

	memcpy((void*)&(this->parameters), (void * )offset, sizeof(BattleCityClientParameters));
}

/**********************************************************************************************************/
// BattleCityCommunicationProtocol
/**********************************************************************************************************/
void * BattleCityCommunicationProtocol::ReceiveDataPacket(Socket socket)
{
    return ReceiveDataPacket(socket.GetConnection().cxSocket);
}

void * BattleCityCommunicationProtocol::ReceiveDataPacket(SOCKET sock)
{
    Socket socket(sock);

    BattleCityDataPacket * packet = NULL;

    SocketPacket headerPacket(PACKET_HEADER_SIZE);

    /// Get Packet Header
    /// int bytesRead = recv ( socket.GetConnection().cxSocket , packetHeader, 3 , 0 );
    int result = socket.Receive(&headerPacket);

    if(result == RES_OK)
    {
        /// if ( bytesRead >= 3 )
        /// {
            char * packetHeader = headerPacket.GetData();

            int packetTotalLength = BattleCityDataPacket::GetSizeFromData(headerPacket.GetData());

            if(packetTotalLength > 0)
            {
                int packetDataLength = packetTotalLength - PACKET_HEADER_SIZE;

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
                        cantRecibida = recv (socket.GetConnection().cxSocket, packetData + PACKET_HEADER_SIZE + totalRecibido, packetDataLength - totalRecibido, 0);

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
                            BattleCityTankPacket * tankPacket = new BattleCityTankPacket(packetData, packetTotalLength);
                            packet = tankPacket;
		                }

                        if ( packetData[0] == BULLET)
		                {
                            BattleCityBulletPacket * bulletPacket = new BattleCityBulletPacket(packetData, packetTotalLength);
                            packet = bulletPacket;
		                }

                        if ( packetData[0] == BOMB)
		                {
                            BattleCityBombPacket * bombPacket = new BattleCityBombPacket(packetData, packetTotalLength);
                            packet = bombPacket;
		                }

                        if ( packetData[0] == WALL)
		                {
                            BattleCityWallPacket * wallPacket = new BattleCityWallPacket(packetData, packetTotalLength);
                            packet = wallPacket;
		                }

                        if ( packetData[0] == COMMAND)
		                {
                            BattleCityCommandPacket * cmdPacket = new BattleCityCommandPacket(packetData, packetTotalLength);
                            packet = cmdPacket;
		                }

						if ( packetData[0] == PLAYERNUMBER)
		                {
                            BattleCityPlayerNumberPacket * playerPacket = new BattleCityPlayerNumberPacket(packetData, packetTotalLength);
                            packet = playerPacket;
		                }

                        if ( packetData[0] == PARAMETERS)
		                {
                            BattleCityParametersPacket * parametersPacket = new BattleCityParametersPacket(packetData, packetTotalLength);
                            packet = parametersPacket;
		                }
                    }

                    delete packetData;
                }
            }
        /// }
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
