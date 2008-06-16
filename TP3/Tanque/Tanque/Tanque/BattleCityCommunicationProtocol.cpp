#ifndef BattleCityCommunicationProtocol_cpp
#define BattleCityCommunicationProtocol_cpp

#include "BattleCityCommunicationProtocol.h"

/**********************************************************************************************************/

BattleCityTankPacket::BattleCityTankPacket(vector<BattleCityTank> tanks)
{
    this->type = TANK; 
    this->buffer[0] = TANK;
    this->size = PACKET_HEADER_SIZE;

    this->tanks = tanks;

    for(int i = 0; i < tanks.size(); i++)
    {
        memcpy(this->buffer + this->size, (void*)&this->tanks[i], sizeof(BattleCityTank));
        this->size += sizeof(BattleCityTank);                
    }
    
    this->SetSizeInBuffer();
};

BattleCityTankPacket::BattleCityTankPacket(char * buffer, int size)
{
    this->type = TANK; 
    this->buffer[0] = TANK;
    this->size = PACKET_HEADER_SIZE;

    memcpy((void *)this->buffer, buffer, size);

    this->size = this->GetSizeFromBuffer();
    int numberOfTanks = (this->size - PACKET_HEADER_SIZE) / sizeof(BattleCityTank);

    unsigned long offset = (unsigned long)this->buffer + PACKET_HEADER_SIZE;

    for(int i = 0; i < numberOfTanks; i++)
    {
        offset += sizeof(BattleCityTank) * i;
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
    this->buffer[0] = BULLET;
    this->size = PACKET_HEADER_SIZE;

    this->bullets = bullets;

    for(int i = 0; i < bullets.size(); i++)
    {
        memcpy(this->buffer + this->size, (void*)&this->bullets[i], sizeof(BattleCityBullet));
        this->size += sizeof(BattleCityBullet);                
    }
    
    this->SetSizeInBuffer();
};

BattleCityBulletPacket::BattleCityBulletPacket(char * buffer, int size)
{
    this->type = BULLET; 
    this->buffer[0] = BULLET;
    this->size = PACKET_HEADER_SIZE;

    memcpy((void *)this->buffer, buffer, size);

    this->size = this->GetSizeFromBuffer();
    int numberOfBullets = (this->size - PACKET_HEADER_SIZE) / sizeof(BattleCityBullet);

    unsigned long offset = (unsigned long)this->buffer + PACKET_HEADER_SIZE;

    for(int i = 0; i < numberOfBullets; i++)
    {
        offset += sizeof(BattleCityBullet) * i;
        BattleCityBullet bullet[1];
        memcpy((void*)bullet, (void *)(offset), sizeof(BattleCityBullet));
        this->bullets.push_back(bullet[0]);
    }
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
            int packetDataLength = packetTotalLength - PACKET_HEADER_SIZE;

            char * packetData = new char[packetTotalLength];
            packetData[0] = packetHeader[0];
            packetData[1] = packetHeader[1];
            packetData[2] = packetHeader[2];

            int tmp;
            int cantRecibida = 0, totalRecibido = 0;

            while ( totalRecibido < packetDataLength )
            {
                cantRecibida = recv (socket, packetData + PACKET_HEADER_SIZE + totalRecibido, packetDataLength - totalRecibido, 0);

                if ( cantRecibida == 0 || cantRecibida == -1 )
                    break;

                totalRecibido += cantRecibida;
                if (totalRecibido >= packetDataLength) 
                    break;
            }

            if(totalRecibido == packetDataLength)
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
            }

            delete packetData;
        }
    }

    return packet;
}

#endif