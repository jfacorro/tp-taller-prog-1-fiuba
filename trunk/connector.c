#include "stdafx.h"
#include "connector.h"
#include "parser.h"
#include "stdlib.h"
#include "string.h"

#define QUIT_STR		"QUIT"
#define EMPTY_STR		"Error! Command not found (STRING, DOUBLE, INT).\n"
#define INCOMPLETE_STR	"Error! Missing parameters.\n"
#define ERROR_STR		"Error! Syntax error. {STRING str, DOUBLE [dn ]+, INT [in ]+}.\n"
#define OK_STR			"Data sent\n"
#define TYPE_STR		"Error! Invalid type.\n"


void __Print ( Connector* connector , enum tr_tipo_dato tipo , int cant , void* data )
{
	char* c;
	double* d;
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
	else if ( tipo == td_int )
	{
		printf ( "< INT " );
		i = (int*) data;
		for ( z = 0 ; z < cant ; z++ )
			printf ( "%lf " , i[z] );
		printf ( "\n" );
	}
	else 
		printf ( TYPE_STR );
}


void __Quit ( Connector* connector , BOOL isReceive )
{
	connector->salir = TRUE;
}


DWORD WINAPI __ThreadReceive ( LPVOID p )
{
	int retval, tipo, cant;
	void* data;
	Connector* connector = (Connector*) p;

	while ( !connector->salir ) 
	{
		retval = trRecibir ( connector->pConexion , td_int , 1 , &tipo );
		if ( retval != RES_OK )
			__Quit ( connector , TRUE );
		else
		{
			retval = trRecibir ( connector->pConexion , td_int , 1 , &cant );
			if ( cant <= 0 || retval != RES_OK )
				__Quit ( connector , TRUE );
			else
			{
				// Una guarda de un byte por si es un string
				data = malloc ( cant + 1 );
				retval = trRecibir ( connector->pConexion , tipo , cant , data );
				__Print ( connector , tipo , cant , data );
				free ( data );
			}
		}
	}

	return retval;
}


void __Parse ( Connector* connector , char* str )
{
	enum ParserResult retval;
	ParserData	result;

	retval = ParseLine ( str , &result );
	if ( retval == PARSER_EMPTY )
		printf ( EMPTY_STR );
	else if ( retval == PARSER_INCOMPLETE )
		printf ( INCOMPLETE_STR );
	else if ( retval == PARSER_ERROR )
		printf ( ERROR_STR );
	else if ( retval == PARSER_OK )
	{
		trEnviar ( connector->pConexion , td_int , 1 , &result.tipo );
		trEnviar ( connector->pConexion , td_int , 1 , &result.cantItems );
		trEnviar ( connector->pConexion , result.tipo , result.cantItems , result.dato );
		printf ( OK_STR );
	}
}


DWORD WINAPI __ThreadSend ( LPVOID p )
{
	char* str;
	int tmp, size;
	Connector* connector;

	connector = (Connector*) p;
	str = NULL;
	size = 0;
	while ( !connector->salir )
	{
		tmp = getchar();	

		if ( str == NULL )	str = malloc ( 1 );
		else				str = realloc ( str , size + 1 );

		if ( tmp != 0x0A )	{
			str[size] = (char) tmp;
			size++;
		} else {
			str[size] = 0;
			if ( !strcmp ( str , QUIT_STR ) )
				__Quit ( connector , FALSE );
			else
				__Parse ( connector , str );

			free ( str );
			str = NULL;
		}			
	}

	return 0;
}


void InitializeConnector ( Connector* connector )
{
	connector->salir = FALSE;

	connector->hThrReceive = CreateThread ( NULL , 0 , __ThreadReceive , 
		(void*) connector , 0 , NULL );

	connector->hThrSend = CreateThread ( NULL , 0 , __ThreadSend ,
		(void*) connector , 0 , NULL );
}


void WaitConnector   ( Connector* connector )
{
	WaitForSingleObject ( connector->hThrReceive , INFINITE );
	WaitForSingleObject ( connector->hThrSend , INFINITE );
}
