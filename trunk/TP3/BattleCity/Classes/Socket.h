#include <winsock2.h>

#ifndef Socket_h

#define Socket_h

typedef struct _CONNECTION
{
    int    cxPuerto;  /* Contiene el valor del puerto de la conexion */
    char*  cxIP;      /* Contiene la direccion IP de la conexion */
    SOCKET cxSocket;  /* Contiene la informacion del socket de la conexion */
} Connection;

/* tipos de datos que se pueden enviar. */
enum DataType { INT_TYPE, CHAR_TYPE, FLOAT_TYPE, DOUBLE_TYPE };

/* Resulados de las funciones.*/
/* Los errores que siguen deben empezar con RES_ seguido de un nombre, por ejemplo RES_TIMEOUT */
#define RES_OK              0
#define RES_ERROR_STARTUP   1
#define RES_INVALID_SOCKET  2
#define RES_ERROR_BIND      3
#define RES_ERROR_LISTEN    4
#define RES_INACTIVE        5
#define RES_ERROR_ACCEPT    6
#define RES_ERROR_CONNECT   7
#define RES_ERROR_SEND      8
#define RES_ERROR_RECEIVE   9
#define RES_ERROR_UNKNOWN  -1

class Packet
{
	private:
		DataType dataType;
		int numberOfItems;
		const void * data;
	public:
		Packet(DataType dataType, int numberOfItems, const void * data)
		{
			this->dataType = dataType;
			this->numberOfItems = numberOfItems;
			this->data = data;
		};

		DataType GetDataType() { return this->dataType; };
		void SetDataType(DataType dataType) { this->dataType = dataType; };

		int GetNumberOfItems() { return this->numberOfItems; };
		void SetNumberOfItems(int numberOfItems) { this->numberOfItems = numberOfItems; };

		const void * GetData() { return this->data; };
		void SetData(const void *  data) { this->data = data; };
};

class Socket
{
	private:
		Connection * connection;

		char * GetIPAddressFromSocket(const struct sockaddr_in sockAddress);
		int GetDataSize (enum DataType dataType);
		
	public:
		Socket() {};

		int Listen(int portNumber);
		int Connect(const char * address, int portNumber);
		int IsActive();
		int Close();

		int Send(Packet packet);
		int Receive(Packet packet);

		char * GetIP();
		int GetPort();
};

#endif
