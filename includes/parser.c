#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "parser.h"

#define STRING_STR	"STRING"
#define DOUBLE_STR	"DOUBLE"
#define FLOAT_STR	"FLOAT"
#define INT_STR		"INT"


int __FindFirstSpace ( char* line , int len , int offset )
{
	int i;
	for ( i = offset ; i < len ; i++ )
		if ( line[i] == ' ' || line[i] == '\t' )
			return i;

	return -1;
}


enum ParseResult __ParseString ( char* line , ParserData* result )
{
	int len;
	
	len = (int) strlen(line);
	if ( len == 0 )
		return PARSER_INCOMPLETE;
	
	result->tipo = td_char;
	result->cantItems = len;
	result->dato = malloc ( len );
	memcpy ( result->dato , line , len );
	return PARSER_OK;
}


enum ParseResult __ParseDouble ( char* line , ParserData* result )
{
	int i, len, firstSpace;
	double tmp;
	
	i = 0;
	len = (int) strlen(line);
	result->cantItems = 0;
	result->tipo = td_double;
	if ( len == 0 )
		return PARSER_INCOMPLETE;

	do
	{
		firstSpace = __FindFirstSpace ( line , len , i );
		
		if ( result->dato == NULL )
			result->dato = malloc ( sizeof(double) );
		else
		{
			result->dato = realloc ( result->dato , 
					sizeof(double) * (result->cantItems + 1) );
		}

		if ( !sscanf ( line + i , "%lf" , &tmp ) )
		{
			free ( result->dato );
			return PARSER_ERROR;
		}

		((double*) result->dato)[result->cantItems] = tmp;
		result->cantItems++;
		i = firstSpace + 1;
	}
	while ( firstSpace != -1 );

	return PARSER_OK;
}


enum ParseResult __ParseFloat ( char* line , ParserData* result )
{
	int i, len, firstSpace;
	float tmp;
	
	i = 0;
	len = (int) strlen(line);
	result->cantItems = 0;
	result->tipo = td_float;
	if ( len == 0 )
		return PARSER_INCOMPLETE;

	do
	{
		firstSpace = __FindFirstSpace ( line , len , i );
		
		if ( result->dato == NULL )
			result->dato = malloc ( sizeof(float) );
		else
		{
			result->dato = realloc ( result->dato , 
					sizeof(float) * (result->cantItems + 1) );
		}

		if ( !sscanf ( line + i , "%f" , &tmp ) )
		{
			free ( result->dato );
			return PARSER_ERROR;
		}

		((float*) result->dato)[result->cantItems] = tmp;
		result->cantItems++;
		i = firstSpace + 1;
	}
	while ( firstSpace != -1 );

	return PARSER_OK;
}

/********************************************************************

********************************************************************/
enum ParseResult __ParseInt ( char* line , ParserData* result )
{
	int i, len, firstSpace;
	int tmp;
	
	i = 0;
	len = (int) strlen(line);
	result->cantItems = 0;
	result->tipo = td_int;
	if ( len == 0 )
		return PARSER_INCOMPLETE;

	do
	{
		firstSpace = __FindFirstSpace ( line , len , i );
		
		if ( result->dato == NULL )
			result->dato = malloc ( sizeof(int) );
		else
		{
			result->dato = realloc ( result->dato , 
					sizeof(int) * (result->cantItems + 1) );
		}

		if ( !sscanf ( line + i , "%d" , &tmp ) )
		{
			free ( result->dato );
			return PARSER_ERROR;
		}

		((int*) result->dato)[result->cantItems] = tmp;
		result->cantItems++;
		i = firstSpace + 1;
	}
	while ( firstSpace != -1 );

	return PARSER_OK;
}


/********************************************************************
* ParseData 
*
*	Parsea una linea de entrada en busca de un tipo de datos y 
* parametros validos. Si estos son correctos, los almacena en forma 
* binaria en result. 
*
* Retorna:
*		PARSER_OK - Se encontraron un tipo y parametros validos
*		PARSER_EMPTY - Linea vacia
*		PARSER_INCOMPLETE - Faltan datos en la linea
*		PARSER_ERROR - El tipo es erroneo o hay un error de sintaxis
*********************************************************************/

enum ParseResult ParseLine ( char* line , ParserData* result )
{
	int len, firstSpace;
	enum ParseResult retval;
	result->dato = NULL;

	len = (int) strlen ( line );
	if ( len == 0 )
		return PARSER_EMPTY;

	firstSpace = __FindFirstSpace ( line , len , 0 );
	if ( firstSpace == -1 )
		return PARSER_INCOMPLETE;

	if ( !strncmp ( line , STRING_STR , firstSpace ) )
		retval = __ParseString ( line + strlen(STRING_STR) + 1 , result );
	else if ( !strncmp ( line , DOUBLE_STR , firstSpace ) )
		retval = __ParseDouble ( line + strlen(DOUBLE_STR) + 1 , result );
	else if ( !strncmp ( line , FLOAT_STR , firstSpace ) )
		retval = __ParseFloat ( line + strlen(FLOAT_STR) + 1 , result );
	else if ( !strncmp ( line , INT_STR , firstSpace ) )
		retval = __ParseInt ( line + strlen(INT_STR) + 1 , result );
	else
		retval = PARSER_ERROR;

	return retval;
}


/**********************************************************
* TestParseData: Testeo automatico de la funcion ParseData
***********************************************************/

#ifdef _DEBUG
#define PRINT_ERROR(str) { ok = 0; printf ( "Test %d failed: %s\n" , i , str ); }
#define CHECK_ERROR(cond,str) if ( cond ) PRINT_ERROR(str);

#define TEST_ERROR(str,x) \
	i++; \
	retval = ParseLine ( str , &result ); \
	CHECK_ERROR(retval != x,str) 

void TestParseData ()
{
	int i, ok;
	enum ParseResult retval;
	ParserData result;

	i = 0, ok = 1;
	TEST_ERROR("", PARSER_EMPTY);
	TEST_ERROR("123456", PARSER_INCOMPLETE);
	TEST_ERROR("STRING", PARSER_INCOMPLETE);
	TEST_ERROR("DOUBLE", PARSER_INCOMPLETE);
	TEST_ERROR("INT", PARSER_INCOMPLETE);
	TEST_ERROR("STRING ", PARSER_INCOMPLETE);
	TEST_ERROR("DOUBLE ", PARSER_INCOMPLETE);
	TEST_ERROR("INT ", PARSER_INCOMPLETE);
	TEST_ERROR("BLG ", PARSER_ERROR);
	TEST_ERROR("INTA " , PARSER_ERROR);
	TEST_ERROR("STRINGA " , PARSER_ERROR);
	TEST_ERROR("DOUBLEA " , PARSER_ERROR);
	TEST_ERROR("DOUBLE 1 A 2", PARSER_ERROR);
	TEST_ERROR("DOUBLE 1 2 B", PARSER_ERROR);
	TEST_ERROR("INT 1 A 2", PARSER_ERROR);
	TEST_ERROR("INT 1 2 B", PARSER_ERROR);

	retval = ParseLine("STRING abcdefghij",&result);
	if ( retval != PARSER_OK || result.tipo != td_char ||
		 result.cantItems != 10 || strncmp(result.dato,"abcdefghij",10) )
		 PRINT_ERROR("STRING abcdefghij");

	retval = ParseLine("STRING a",&result);
	if ( retval != PARSER_OK || result.tipo != td_char ||
		 result.cantItems != 1 || strncmp(result.dato,"a",1) )		 
		 PRINT_ERROR("STRING a");

	retval = ParseLine("DOUBLE 1 2 3",&result);
	if ( retval != PARSER_OK || result.tipo != td_double ||
		 result.cantItems != 3 || 
		 ((double*) result.dato) [0] != 1 ||
		 ((double*) result.dato) [1] != 2 ||
		 ((double*) result.dato) [2] != 3 )
		PRINT_ERROR("DOUBLE 1 2 3");

	retval = ParseLine("DOUBLE 1.5 2.3 0.54",&result);
	if ( retval != PARSER_OK || result.tipo != td_double ||
		 result.cantItems != 3 || 
		 ((double*) result.dato) [0] != 1.5 ||
		 ((double*) result.dato) [1] != 2.3 ||
		 ((double*) result.dato) [2] != 0.54 )
		PRINT_ERROR("DOUBLE 1.5 2.3 0.54");

	retval = ParseLine("INT 1 2 3",&result);
	if ( retval != PARSER_OK || result.tipo != td_int ||
		 result.cantItems != 3 || 
		 ((int*) result.dato) [0] != 1 ||
		 ((int*) result.dato) [1] != 2 ||
		 ((int*) result.dato) [2] != 3 )
		PRINT_ERROR("INT 1 2 3");

	if ( ok )
		printf ( "TestParseData: All tests ok\n" );
}

#undef TEST_ERROR
#endif
