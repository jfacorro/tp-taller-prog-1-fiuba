#ifndef __PARSER_INCLUDED
#define __PARSER_INCLUDED

#include "transferencias.h"


enum ParserResult { 
	PARSER_OK , 
	PARSER_EMPTY , 
	PARSER_INCOMPLETE , 
	PARSER_ERROR 
};


typedef struct _ParserData {
	enum tr_tipo_dato	tipo;
	int					cantItems;
	void*				dato;
}
	ParserData;


/********************************************************************
* ParseData 
*
*	Parsea una linea de entrada en busca de un tipo de datos y 
* parametros validos. Si estos son correctos, los almacena en forma 
* binaria en result. NOTA: Puede modificar la linea de entrada.
*
* Retorna:
*		PARSER_OK - Se encontraron un tipo y parametros validos
*		PARSER_EMPTY - Linea vacia
*		PARSER_INCOMPLETE - Faltan datos en la linea
*		PARSER_ERROR - El tipo es erroneo o hay un error de sintaxis
*********************************************************************/

enum ParserResult ParseLine ( char* line , ParserData* result );


/**********************************************************
* TestParseData: Testeo automatico de la funcion ParseData
***********************************************************/

#ifdef _DEBUG
void TestParseData ();
#endif

#endif
