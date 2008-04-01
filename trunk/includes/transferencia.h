#ifndef _TRANSFERENCIA_H_
#define _TRANSFERENCIA_H_

#include <winsock2.h>

typedef struct _CONEXION
{
    int    cxPuerto;  /* Contiene el valor del puerto de la conexion */
    char*  cxIP;      /* Contiene la direccion IP de la conexion */
    SOCKET cxSocket;  /* Contiene la informacion del socket de la conexion */
} CONEXION;

/* tipos de datos que se pueden enviar. */
enum tr_tipo_dato { td_int, td_char, td_float, td_double };

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

/*****************************************************************/
/* trEscuchar: Escucha en un Puerto por conexiones entrantes.    */
/* 	           devuelve RES_OK si alguien se conect� y pConexion */
/* 	           apunta a la nueva conexion. 			             */
/* 	           De lo contrario devuelve un codigo de error y	 */
/* 	           pConexion permanece inalterado. 			         */
/*****************************************************************/
int trEscuchar(int Puerto, CONEXION *pConexion);

/*****************************************************************/
/* trConectar: Intenta conectarse a una direcci�n de internet 	 */
/* 	           y a un Puerto de comunicaci�n espec�fico. 	     */
/* 	           Si todo sale bien devuelve RES_OK y pConexion 	 */
/* 	           apunta a la nueva conexion. De lo contrario 	     */
/* 	           devuelve un codigo de error y pConexion permanece */
/* 	           inalterado. 					                     */
/*****************************************************************/ 
int trConectar(const char *pDireccion, int Puerto, CONEXION *pConexion);

/*****************************************************************/
/* trEnviar: Envia a traves de la conexion una cantidad de 	     */
/* 	         datos de un tipo de datos especificado 		     */
/* 	         Si todo sale bien devuelve RES_OK de lo contrario   */
/* 	         devuelve un codigo de error. 			             */
/*****************************************************************/ 
int trEnviar(CONEXION *pConexion, enum tr_tipo_dato tipo, int cantItems, const void *datos);

/*****************************************************************/
/* trRecibir: Recibe a traves de la conexion una cantidad de 	 */
/*            datos de un tipo de datos especificado 		     */
/* 	          Si todo sale bien devuelve RES_OK de lo contrario  */
/* 	          devuelve un codigo de error. 			             */
/*****************************************************************/ 
int trRecibir(CONEXION *pConexion, enum tr_tipo_dato tipo, int cantItems, void *datos);

/*****************************************************************/
/* trCerrarConexion: cierra una conexion previamente abierta. 	 */
/* 	           Si todo sale bien devuelve RES_OK de lo contrario */
/* 	           devuelve un codigo de error.                      */
/*****************************************************************/
int trCerrarConexion(CONEXION *pConexion);

/*****************************************************************/
/* trConexionActiva: verifica que una conexi�n est� activa. 	 */
/* 	        Si la conexion est� activa devuelve RES_OK de lo 	 */
/* 	        contrario devuelve un codigo de error. 		         */
/*****************************************************************/
int trConexionActiva(CONEXION *pConexion);

/******************************************************************/
/* trIP: copiar la direcci�n con la que se ha establecido 	      */
/*       la conexi�n en pIP 					                  */
/* 	     devuelve RES_OK si todo sale bien de lo contrario 	      */
/* 	     devuelve un codigo de error. 				              */
/******************************************************************/
int trIP(CONEXION *pConexion, char *pIP);

/******************************************************************/
/* trPort: copiar el puerto de comunicaci�n en pPuerto. 	      */
/*         devuelve RES_OK si todo sale bien de lo contrario 	  */
/*         devuelve un c�digo de error. 			              */
/******************************************************************/
int trPuerto(CONEXION *pConexion, int *pPuerto);

#endif

