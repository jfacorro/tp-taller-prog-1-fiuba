#ifndef Socket_h

#define Socket_h

#include <winsock2.h>

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

class SocketPacket
{
	protected:
		int size;
		char * data;

	public:
        SocketPacket()
        {
            this->size = 0;
            this->data = NULL;
        };

        SocketPacket(int size)
        {
            this->size = size;
            this->data = new char[size];
        };
        
        SocketPacket(char * data, int size)
		{
			this->size = size;
            this->data = data;
		};

        ~SocketPacket()
        {
            if(this->data != NULL)
            {
                delete [] this->data;
                this->data = NULL;
            }
        }

		int GetSize() { return this->size; };
		void SetSize(int size) { this->size = size; };

		char * GetData() { return this->data; };
};

class Socket
{
	private:
		Connection connection;

		char * GetIPAddressFromSocket(const struct sockaddr_in sockAddress);
		
	public:
		Socket() {};

        Socket(SOCKET sock) 
        {
            this->connection.cxSocket = sock;
        };

		int Listen(int port, int backlog);
        int Accept(Socket * acceptedSocket);
		int Connect(const char * address, int portNumber);
		int IsActive();
		int Close();

		int Send(SocketPacket packet);
		int Receive(SocketPacket * packet);

		char * GetIP();
		int GetPort();

        SOCKET GetSocket() { return this->connection.cxSocket; };
        Connection GetConnection() { return this->connection;};
        void SetConnection(Connection conn) { this->connection = conn; };
};

#endif
