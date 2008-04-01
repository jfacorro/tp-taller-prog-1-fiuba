//#include "stdafx.h"
#include "stdio.h"
#include "connector.h"
#include "parser.h"
#include "stdlib.h"
#include "string.h"

#define QUIT_STR		"QUIT"
#define EMPTY_STR		"Error! Command not found (STRING, DOUBLE, FLOAT, INT).\n"
#define INCOMPLETE_STR	"Error! Missing parameters.\n"
#define ERROR_STR		"Error! Syntax error. {STRING str, DOUBLE [dn ]+, FLOAT [dn ]+, INT [in ]+}.\n"
#define OK_STR			"Data sent\n"
#define TYPE_STR		"Error! Invalid type.\n"


/************************************************************************
 _GetDataSize:	Resuelve el tamaño de los datos a recibir (enviados)
				en base al tipo.
************************************************************************/
int _GetDataSize ( enum tr_tipo_dato tipo )
{
	if ( tipo == td_int ) 
		return sizeof(int);
	else if (tipo == td_double)
		return sizeof(double);
	else if (tipo == td_char)
		return sizeof(char);
	else if (tipo == td_float)
		return sizeof(float);
	else
		return 0;
}


/************************************************************************
 __Print: Imprime a pantalla los datos que se le pasan.
************************************************************************/
void __Print ( Connector* connector , enum tr_tipo_dato tipo , int cant , void* data )
{
	char* c;
	double* d;
	float* f;
	int* i;
	int z;

	if ( tipo == td_char )
	{
		c = (char*) data;
		c[cant] = 0;
		printf ( "< STRING %s\n" , c );
	}
	else if ( tipo == td_double )
	{
		printf ( "< DOUBLE " );
		d = (double*) data;
		for ( z = 0 ; z < cant ; z++ )
			printf ( "%lf " , d[z] );
		printf ( "\n" );
	}
	else if ( tipo == td_float )
	{
		printf ( "< FLOAT " );
		f = (float*) data;
		for ( z = 0 ; z < cant ; z++ )
			printf ( "%f " , f[z] );
		printf ( "\n" );
	}
	else if ( tipo == td_int )
	{
		printf ( "< INT " );
		i = (int*) data;
		for ( z = 0 ; z < cant ; z++ )
			printf ( "%d " , i[z] );
		printf ( "\n" );
	}
	else 
		printf ( TYPE_STR );
}

/************************************************************************
 __Quit: Método de salida. Setea lo s
************************************************************************/
void __Quit ( Connector* connector , BOOL isReceive )
{
	trCerrarConexion ( connector->pConexion );
	connector->salir = TRUE;

	if ( isReceive )
		TerminateThread ( connector->hThrSend , 0 );
	else
		TerminateThread ( connector->hThrReceive , 0 );
}


/************************************************************************
 __ThreadReceive:	Metodo asociado al thread de recpcion de datos.
************************************************************************/
DWORD WINAPI __ThreadReceive ( LPVOID conn )
{
	int retval; // Contiene el resultado de la recepcio de datos
	int tipo; // Contiene el tipo de datos que se recibe
	int cant; // Contiene la cantidad de items a recibir
	void* data; // Contiene los datos recibidos
	
	Connector* connector = (Connector*) conn; // Contiene el connector previamente inicializado

	// Antes de recibir cualquier dato verificar que 
	// no se recibio el comando de salida.
	while ( !connector->salir ) 
	{
		// Recibir el tipo de dato
		retval = trRecibir ( connector->pConexion , td_int , 1 , &tipo );
		// Si hubo algun error, lamar al método de salida.
		if ( retval != RES_OK )
			__Quit ( connector , TRUE );
		else
		{
			// Recibir la cantidad de items enviados
			retval = trRecibir ( connector->pConexion , td_int , 1 , &cant );
			// Si hubo algun error, lamar al método de salida.
			if ( cant <= 0 || retval != RES_OK )
				__Quit ( connector , TRUE );
			else
			{
				// Reservar el espacio correspondiente para la cantidad
				// y el tipo de datos a recibir, con una guarda de un byte 
				// por si el tipo es STRING
				data = malloc ( cant * _GetDataSize (tipo) + 1 );
				// Recibir los datos enviados
				retval = trRecibir ( connector->pConexion , tipo , cant , data );
				// Imprimirlos a pantalla
				__Print ( connector , tipo , cant , data );
				// Liberar memoria
				free ( data );
			}
		}
	}

	return retval;
}


/************************************************************************
 __Parse:	Asegura que la cadena ingresada tenga un formato
			válido, interpreta los datos de ésta y los envia por medio
			del connector.
************************************************************************/
void __Parse ( Connector* connector , char* str )
{
	enum ParserResult retval;	// Contiene el resultado de la operacion
								// de validacion y parseo
	ParserData	result;			// Contiene los datos parseados
								// de la cadena ingresada

	// Parsear cadena de caracteres
	retval = ParseLine ( str , &result );

	// Manejar resultado
	if ( retval == PARSER_EMPTY )
		printf ( EMPTY_STR );
	else if ( retval == PARSER_INCOMPLETE )
		printf ( INCOMPLETE_STR );
	else if ( retval == PARSER_ERROR )
		printf ( ERROR_STR );
	else if ( retval == PARSER_OK )
	{
		// Enviar datos:
		trEnviar ( connector->pConexion , td_int , 1 , &result.tipo ); // Enviar tipo de dato
		trEnviar ( connector->pConexion , td_int , 1 , &result.cantItems ); // Enviar cantidad de items
		trEnviar ( connector->pConexion , result.tipo , result.cantItems , result.dato ); // Enviar cantidad de items
		printf ( OK_STR );
	}
}

/************************************************************************
 __ThreadSend: Metodo asociado al thread para el envio de datos.
************************************************************************/
DWORD WINAPI __ThreadSend ( LPVOID conn )
{
	char* str; // Contiene los datos a enviar
	int tmp, size;
	Connector* connector; // Contiene el connector previamente inicializado

	connector = (Connector*) conn;
	str = NULL;
	size = 0;

	// Antes de enviar cualquier dato verificar que 
	// no se recibio el comando de salida.
	while ( !connector->salir )
	{
		// Leer caracter del input del usuario
		tmp = getchar();

		// Resevar el espacio de memoria correspondiente
		if ( str == NULL )
			str = malloc ( 1 );
		else
			str = realloc ( str , size + 1 );

		// Mientras que el caracter no sea EOL seguir leyendo 
		if ( tmp != 0x0A )	{
			str[size] = (char) tmp;
			size = size + 1;
		} else {
			str[size] = 0;
			// Si la cadena ingresada es el comando
			// QUIT, llamar al metodo de cierre del connector
			if ( !strcmp ( str , QUIT_STR ) )
				__Quit ( connector , FALSE );
			else
				__Parse ( connector , str );

			// Liberar memoria y resetear 
			// valores de las variables
			free ( str );
			str = NULL;
			size = 0;
		}			
	}

	return 0;
}


/************************************************************************
 InitializeConnector:	Inicializa el connector, asignandole la conexion
						de escucha establecida previamente para el
						servidor; y creando los threadspara recibir
						y enviar datos.
************************************************************************/
void InitializeConnector ( Connector* connector , CONEXION* pConexion )
{		
	connector->salir = FALSE;
	connector->pConexion = pConexion;

	/* Crear thread de recepcion de datos */
	connector->hThrReceive = CreateThread ( NULL , 0 , __ThreadReceive , 
		(void*) connector , 0 , NULL );

	/* Crear thread de envio de datos */
	connector->hThrSend = CreateThread ( NULL , 0 , __ThreadSend ,
		(void*) connector , 0 , NULL );
}

/************************************************************************
 WaitConnector:	Activa los threads previamente creados para recibir
				y enviar datos.
************************************************************************/
void WaitConnector   ( Connector* connector )
{
	/* Activar thread de recepcion de datos */
	WaitForSingleObject ( connector->hThrReceive , INFINITE );
	/* Activar thread de envio de datos */
	WaitForSingleObject ( connector->hThrSend , INFINITE );
}
