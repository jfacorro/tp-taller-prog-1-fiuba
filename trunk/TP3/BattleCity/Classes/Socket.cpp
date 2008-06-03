#include "Socket.h"

#ifndef Socket_cpp

#define Socket_cpp

#define NULL_PORT	-1 /* Se utiliza como valor nulo de puerto */
#define IP_SIZE		16 /* Longitud maxima de una direccion IP */


int Socket::Listen(int port)
{
	WSADATA wsaData; /* Utilizada para inicializacion del winsock */

	WORD wVersionReq = MAKEWORD(2, 2); /* Contiene la version */

	SOCKET sock; /* Contiene la informacion del socket */

	SOCKET sockAceptado; /* Contiene la informacion del socket que acepto */   

	struct sockaddr_in sockAddrIn; /* Direccion de socket */

	int tamSockAddrIn; /* Se utiliza para aceptar el mensaje y obtener su info */

	int codError = 0; /* Codigo de error que se evalua dentro de la funcion */

	int resultado = RES_ERROR_UNKNOWN; /* Codigo de error que se devolvera */

	/* Inicializa el socket */
	codError = WSAStartup(wVersionReq, &wsaData);
	
	if (codError != 0) 
	{
	  resultado = RES_ERROR_STARTUP;
	} 
	else 
	{
		/* Crea un socket */
		sock = socket(AF_INET, SOCK_STREAM, 0);

		if (sock == INVALID_SOCKET) 
		{
			resultado = RES_INVALID_SOCKET; 
		} 
		else 
		{
			/* Carga los valores de sockAddrIn */
			sockAddrIn.sin_family = AF_INET;     /* Protocolo ipv4 */
			sockAddrIn.sin_port = htons(port); /* Puerto indicado */
			sockAddrIn.sin_addr.s_addr = INADDR_ANY; /* Cualquier direccion IP */
         
			codError = bind(sock, (SOCKADDR*) &sockAddrIn, sizeof(sockAddrIn));
			if (codError != 0) 
			{
				resultado = RES_ERROR_BIND;
			} 
			else 
			{
				/* Escucha una sola conexion */
				codError = listen(sock, 1);
				if (codError != 0) 
				{
					resultado = RES_ERROR_LISTEN;
				} 
				else 
				{
					/* Acepta la conexion */
					sockAceptado = SOCKET_ERROR;
					tamSockAddrIn = sizeof(sockAddrIn);
					sockAceptado = accept(sock, (struct sockaddr*)&sockAddrIn, &tamSockAddrIn);

					if (sockAceptado == SOCKET_ERROR) 
					{
						resultado = RES_ERROR_ACCEPT;
					}
					else 
					{
						this->connection.cxPuerto = port;
						this->connection.cxIP = (char*) malloc(IP_SIZE);
						this->connection.cxIP = GetIPAddressFromSocket(sockAddrIn);
						this->connection.cxSocket = sockAceptado;
						resultado = RES_OK;
					}
				}
			}
      }
   }

    return resultado;
}

int Socket::Connect(const char * address, int port) 
{
   WSADATA wsaData; /* Utilizada para inicializacion del winsock */

   WORD wVersionReq = MAKEWORD(2, 2); /* Contiene la version */

   SOCKET sock; /* Contiene la informacion del socket */

   struct hostent* hostInfo; /* Se utiliza para convertir el nombre del host a su direccion IP */

   struct sockaddr_in sockAddrIn; /* Direccion de socket */

   int codError = 0; /* Codigo de error que se evalua dentro de la funcion */

   int resultado = RES_ERROR_UNKNOWN; /* Codigo de error que se devolvera */

   /* Inicializa el socket */
	codError = WSAStartup(wVersionReq, &wsaData);
	if (codError != 0) 
	{
		resultado = RES_ERROR_STARTUP;
	}
	else 
	{
		/* Crea un socket */
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) 
		{
			resultado = RES_INVALID_SOCKET;
		} 
		else 
		{
			/* Carga los valores de sockAddrIn */
			sockAddrIn.sin_family = AF_INET;       /* Protocolo ipv4 */
			sockAddrIn.sin_port = htons(port);   /* Puerto indicado */
			hostInfo = gethostbyname(address);  /* Direccion IP */
			sockAddrIn.sin_addr = *((struct in_addr *)((*hostInfo).h_addr));
         
			codError = connect(sock, (SOCKADDR *)(&sockAddrIn), sizeof(sockAddrIn));

			if (codError != 0) 
			{
				resultado = RES_ERROR_CONNECT;
			} 
			else 
			{
				this->connection.cxPuerto = port;
				this->connection.cxIP = (char*) malloc(IP_SIZE);
				this->connection.cxIP = GetIPAddressFromSocket(sockAddrIn);
				this->connection.cxSocket = sock;

				resultado = RES_OK;
			}
		}
	}

	return resultado;
}

int Socket::Send(Packet packet) 
{
	int tmp;
	int resultado = RES_ERROR_UNKNOWN;
	int cantEnviada = 0;
   
	tmp = GetDataSize(packet.GetDataType());
	cantEnviada = send(this->connection.cxSocket, (char *)packet.GetData(), packet.GetNumberOfItems() * tmp, 0);

	if (cantEnviada != packet.GetNumberOfItems() * tmp)  /* No envio la cantidad indicada */
	{
		resultado = RES_ERROR_SEND;
	} 
	else 
	{
		resultado = RES_OK;
	}

	return resultado;
}

int Socket::Receive(Packet packet) 
{
   int tmp;
   int resultado = RES_ERROR_UNKNOWN;
   int cantRecibida = 0, totalRecibido = 0, tamano;   

   tmp = this->GetDataSize(packet.GetDataType());
   tamano = packet.GetNumberOfItems() * tmp;
   while ( totalRecibido < tamano )
   {
		cantRecibida = recv (this->connection.cxSocket, (char *)((char *)packet.GetData() + totalRecibido), tamano - totalRecibido, 0);
	
		if ( cantRecibida == 0 || cantRecibida == -1 )
			return RES_ERROR_RECEIVE;

		totalRecibido += cantRecibida;
		if (totalRecibido >= tamano) 
			break;
   }

   return RES_OK;
}

int Socket::Close() 
{
	int resultado = RES_ERROR_UNKNOWN;
   
	if (this->IsActive() == RES_OK) /* La conexion esta activa */
	{ 
		/* Cierra el socket y quita los valores de la conexion */
		if (this->connection.cxIP != NULL) 
		{
			free(this->connection.cxIP); 
		}
		this->connection.cxPuerto = NULL_PORT;
		closesocket(this->connection.cxSocket);
		this->connection.cxSocket = INVALID_SOCKET;
		resultado = RES_OK;
	} 
	else 
	{ /* La conexion no esta activa */
		resultado = RES_INACTIVE;
	}
   
	return resultado;
}

int Socket::IsActive() 
{
	int resultado = RES_ERROR_UNKNOWN;
   
	if (this->connection.cxPuerto != 0) 
	{ /* La conexion no es nula */
		if (this->connection.cxSocket != INVALID_SOCKET) 
		{ /* La conexion esta activa */
			resultado = RES_OK;
		} 
		else 
		{ /* La conexion no esta activa */
			resultado = RES_INACTIVE;      
		}
	} 
	else 
	{ /* La conexion es nula */
		resultado = RES_INACTIVE;   
	}

	return resultado;
}

char * Socket::GetIPAddressFromSocket(const struct sockaddr_in sockAddress)
{
    char* charTemp = (char*) malloc(3);
	char* dirIP = new char[IP_SIZE];

    /* Inicializa el valor de la direccion */
    strcpy(dirIP,"");
    /* Obtiene el primer grupo de números de la direccion IP */
    itoa((int)sockAddress.sin_addr.S_un.S_un_b.s_b1, charTemp, 10);
    strcat(dirIP, charTemp);
    strcat(dirIP, ".");
    /* Obtiene el segundo grupo de números de la direccion IP */
    itoa((int)sockAddress.sin_addr.S_un.S_un_b.s_b2, charTemp,10);
    strcat(dirIP, charTemp);
    strcat(dirIP, ".");
    /* Obtiene el tercer grupo de números de la direccion IP */
    itoa((int)sockAddress.sin_addr.S_un.S_un_b.s_b3, charTemp, 10);
    strcat(dirIP, charTemp);
    strcat(dirIP, ".");
    /* Obtiene el cuarto grupo de números de la direccion IP */
    itoa((int)sockAddress.sin_addr.S_un.S_un_b.s_b4, charTemp, 10);
    strcat(dirIP, charTemp);
    strcat(dirIP, "\0");   

	return dirIP;
}

int Socket::GetDataSize(enum DataType dataType)
{
	if ( dataType == INT_TYPE ) 
		return sizeof(int);
	else if (dataType == DOUBLE_TYPE)
		return sizeof(double);
	else if (dataType == CHAR_TYPE)
		return sizeof(char);
	else if (dataType == FLOAT_TYPE)
		return sizeof(float);
	else
		return 0;
}


#endif


