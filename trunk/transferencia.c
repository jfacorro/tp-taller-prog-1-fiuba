#include "transferencia.h"

/* Constante utilizada como valor nulo de referencias */
#define NULL  0

/* Constantes utilizadas para valores de la conexion */
#define PUERTO_NULO -1 /* Se utiliza como valor nulo de puerto */
#define TAMANIO_IP  16 /* Longitud maxima de una direccion IP */


/*****************************************************************/
/* trEscuchar: Escucha en un Puerto por conexiones entrantes.    */
/* 	           devuelve RES_OK si alguien se conectó y pConexion */
/* 	           apunta a la nueva conexion. 			             */
/* 	           De lo contrario devuelve un codigo de error y	 */
/* 	           pConexion permanece inalterado. 			         */
/*****************************************************************/
int trEscuchar(int Puerto, CONEXION *pConexion) {
   WSADATA wsaData; /* Utilizada para inicializacion del winsock */
   WORD wVersionReq = MAKEWORD(2, 2); /* Contiene la version */
   SOCKET sock; /* Contiene la informacion del socket */
   SOCKET sockAceptado; /* Contiene la informacion del socket que acepto */   
   struct hostent* hostInfo; /* Se utiliza para convertir el nombre del host a su direccion IP */
   struct sockaddr_in sockAddrIn; /* Direccion de socket */
   int tamSockAddrIn; /* Se utiliza para aceptar el mensaje y obtener su info */
   int codError = 0; /* Codigo de error que se evalua dentro de la funcion */
   int resultado = RES_ERROR_UNKNOWN; /* Codigo de error que se devolvera */

   /* Inicializa el socket */
   codError = WSAStartup(wVersionReq, &wsaData);
   if (codError != 0) {
      resultado = RES_ERROR_STARTUP;

   } else {
      /* Crea un socket */
      sock = socket(AF_INET, SOCK_STREAM, 0);
      if (sock == INVALID_SOCKET) {
         resultado = RES_INVALID_SOCKET;
         
      } else {
         /* Carga los valores de sockAddrIn */
         sockAddrIn.sin_family = AF_INET;     /* Protocolo ipv4 */
         sockAddrIn.sin_port = htons(Puerto); /* Puerto indicado */
         sockAddrIn.sin_addr.s_addr = INADDR_ANY; /* Cualquier direccion IP */
         
         codError = bind(sock, (SOCKADDR*) &sockAddrIn, sizeof(sockAddrIn));
         if (codError != 0) {
            resultado = RES_ERROR_BIND;

         } else {
            /* Escucha una sola conexion */
            codError = listen(sock, 1);
            if (codError != 0) {
               resultado = RES_ERROR_LISTEN;
            } else {
               /* Acepta la conexion */
               sockAceptado = SOCKET_ERROR;
               tamSockAddrIn = sizeof(sockAddrIn);
               sockAceptado = accept(sock, (struct SOCKADDR*)&sockAddrIn, &tamSockAddrIn);

               if (sockAceptado == SOCKET_ERROR) {
                  resultado = RES_ERROR_ACCEPT;
               } else {
                  (*pConexion).cxPuerto = Puerto;
                  (*pConexion).cxIP = (char*) malloc(TAMANIO_IP);
                  obtenerDirIP((*pConexion).cxIP, sockAddrIn);
                  (*pConexion).cxSocket = sock;
                  resultado = RES_OK;
               }
            }
         }
      }
   }

    return resultado;
}

/*****************************************************************/
/* trConectar: Intenta conectarse a una dirección de internet 	 */
/* 	           y a un Puerto de comunicación específico. 	     */
/* 	           Si todo sale bien devuelve RES_OK y pConexion 	 */
/* 	           apunta a la nueva conexion. De lo contrario 	     */
/* 	           devuelve un codigo de error y pConexion permanece */
/* 	           inalterado. 					                     */
/*****************************************************************/ 
int trConectar(const char *pDireccion, int Puerto, CONEXION *pConexion) {
   WSADATA wsaData; /* Utilizada para inicializacion del winsock */
   WORD wVersionReq = MAKEWORD(2, 2); /* Contiene la version */
   SOCKET sock; /* Contiene la informacion del socket */
   SOCKET tempSock; /* Se utiliza para no sobreescribir informacion al aceptar la conexion */   
   struct hostent* hostInfo; /* Se utiliza para convertir el nombre del host a su direccion IP */
   struct sockaddr_in sockAddrIn; /* Direccion de socket */
   int codError = 0; /* Codigo de error que se evalua dentro de la funcion */
   int resultado = RES_ERROR_UNKNOWN; /* Codigo de error que se devolvera */

   /* Inicializa el socket */
   codError = WSAStartup(wVersionReq, &wsaData);
   if (codError != 0) {
      resultado = RES_ERROR_STARTUP;

   } else {
      /* Crea un socket */
      sock = socket(AF_INET, SOCK_STREAM, 0);
      if (sock == INVALID_SOCKET) {
         resultado = RES_INVALID_SOCKET;
         
      } else {
         /* Carga los valores de sockAddrIn */
         sockAddrIn.sin_family = AF_INET;       /* Protocolo ipv4 */
         sockAddrIn.sin_port = htons(Puerto);   /* Puerto indicado */
         hostInfo = gethostbyname(pDireccion);  /* Direccion IP */
         sockAddrIn.sin_addr = *((struct in_addr *)((*hostInfo).h_addr));
         
            codError = connect(sock, (SOCKADDR *)(&sockAddrIn), sizeof(sockAddrIn));
            if (codError != 0) {
               resultado = RES_ERROR_CONNECT;
            } else {
               (*pConexion).cxPuerto = Puerto;
               (*pConexion).cxIP = (char*) malloc(TAMANIO_IP);
               obtenerDirIP((*pConexion).cxIP, sockAddrIn);
               (*pConexion).cxSocket = sock;

               resultado = RES_OK;
            }

      }
   }

    return resultado;
}

/*****************************************************************/
/* trEnviar: Envia a traves de la conexion una cantidad de 	     */
/* 	         datos de un tipo de datos especificado 		     */
/* 	         Si todo sale bien devuelve RES_OK de lo contrario   */
/* 	         devuelve un codigo de error. 			             */
/*****************************************************************/ 
int trEnviar(CONEXION *pConexion, enum tr_tipo_dato tipo, int cantItems, const void *datos) {
   int resultado = RES_ERROR_UNKNOWN;
   int cantEnviada = 0;
   
   cantEnviada = send((*pConexion).cxSocket, datos, cantItems, 0);
   if (cantEnviada != cantItems) { /* No envio la cantidad indicada */
      resultado = RES_ERROR_SEND;
   } else {
      resultado = RES_OK;
   }

   return resultado;
}

/*****************************************************************/
/* trRecibir: Recibe a traves de la conexion una cantidad de 	 */
/*            datos de un tipo de datos especificado 		     */
/* 	          Si todo sale bien devuelve RES_OK de lo contrario  */
/* 	          devuelve un codigo de error. 			             */
/*****************************************************************/ 
int trRecibir(CONEXION *pConexion, enum tr_tipo_dato tipo, int cantItems, void *datos) {
   int resultado = RES_ERROR_UNKNOWN;
   int cantRecibida = 0;   

   cantRecibida = recv ((*pConexion).cxSocket, datos, cantItems, 0);
   if (cantRecibida != cantItems) { /* No recibio la cantidad indicada */
      resultado = RES_ERROR_RECEIVE;
   } else {
      resultado = RES_OK;
   }

   return resultado;
}

/*****************************************************************/
/* trCerrarConexion: cierra una conexion previamente abierta. 	 */
/* 	           Si todo sale bien devuelve RES_OK de lo contrario */
/* 	           devuelve un codigo de error.                      */
/*****************************************************************/
int trCerrarConexion(CONEXION *pConexion) {
   int resultado = RES_ERROR_UNKNOWN;
   
   if (trConexionActiva(pConexion) == RES_OK) { /* La conexion esta activa */
      /* Cierra el socket y quita los valores de la conexion */
      free ((*pConexion).cxIP);
      (*pConexion).cxPuerto = PUERTO_NULO;
      closesocket((*pConexion).cxSocket);
      (*pConexion).cxSocket = NULL;      
      resultado = RES_OK;
   } else { /* La conexion no esta activa */
      resultado = RES_INACTIVE;
   }
   
   return resultado;
}

/*****************************************************************/
/* trConexionActiva: verifica que una conexión esté activa. 	 */
/* 	        Si la conexion está activa devuelve RES_OK de lo 	 */
/* 	        contrario devuelve un codigo de error. 		         */
/*****************************************************************/
int trConexionActiva(CONEXION *pConexion) {
   int resultado = RES_ERROR_UNKNOWN;
   
   if (pConexion != NULL) { /* La conexion no es nula */
      if ((*pConexion).cxSocket != NULL) { /* La conexion esta activa */
         resultado = RES_OK;
      } else { /* La conexion no esta activa */
         resultado = RES_INACTIVE;      
      }
   } else { /* La conexion es nula */
      resultado = RES_INACTIVE;   
   }

   return resultado;
}

/******************************************************************/
/* trIP: copiar la dirección con la que se ha establecido 	      */
/*       la conexión en pIP 					                  */
/* 	     devuelve RES_OK si todo sale bien de lo contrario 	      */
/* 	     devuelve un codigo de error. 				              */
/******************************************************************/
int trIP(CONEXION *pConexion, char *pIP) {
   int resultado = RES_ERROR_UNKNOWN;

   if (trConexionActiva(pConexion) == RES_OK) { /* La conexion esta activa */
      strcpy(pIP, (*pConexion).cxIP);
      if (strcmp(pIP, (*pConexion).cxIP) == 0) { /* Se copio exitosamente */
         resultado = RES_OK;
      }
   } else { /* La conexion es nula */
      resultado = RES_INACTIVE;   
   }      

   return resultado;
}

/******************************************************************/
/* trPort: copiar el puerto de comunicación en pPuerto. 	      */
/*         devuelve RES_OK si todo sale bien de lo contrario 	  */
/*         devuelve un código de error. 			              */
/******************************************************************/
int trPuerto(CONEXION *pConexion, int *pPuerto) {
   int resultado = RES_ERROR_UNKNOWN;
   
   if (trConexionActiva(pConexion) == RES_OK) { /* La conexion esta activa */
      *pPuerto = (*pConexion).cxPuerto;
      if (*pPuerto == (*pConexion).cxPuerto) { /* Se copio exitosamente */
         resultado = RES_OK;
      }
   } else { /* La conexion es nula */
      resultado = RES_INACTIVE;   
   }   

   return resultado;
}


/* FUNCIONES AUXILIARES *******************************************************/
/******************************************************************************/
/* Obtiene una direccion IP en el formato char* de una estructura sockaddr_in */
/* PRE: dirIP debe tener el espacio necesario reservado                       */
/******************************************************************************/
void obtenerDirIP(char* dirIP, const struct sockaddr_in sockAddress) {
    char* charTemp = (char*) malloc(3);

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
}
