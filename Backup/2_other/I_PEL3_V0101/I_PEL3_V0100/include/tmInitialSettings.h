/*===========================================================================================

    File Name       : tmFileAccess.h
					  tmInitialSettings.h  ( 2018/01/04 Modified name from tmFileAccess.h )

    Version         : V1_00_a

    Built Date      : 2016/10/21

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2016/10/21 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_INITIALSETTINGS_H
#define TM_INITIALSETTINGS_H

#include "windows.h"
#include <process.h>
#include "String.h"
#include <sstream>
#include <vector>

using namespace System;
using namespace IO;
using namespace std;



enum{
	INIT_COM_NAME	= 0,
	INIT_BAUDRATE,
	INIT_PROTOCOL,
	INIT_ISRS485,
	INIT_SLAVEID,
	//INIT_FILE,
	INIT_HWEEP_PATH,
	INIT_FWEEP_PATH,
	INIT_NUM
};


typedef struct{

	string		Items[ INIT_NUM ];

}Struct_Initial;

/*===========================================================================================
    Function Name    : tmInit_OpenFile
    Input            : filename: filename
    Return           : 1 = ok, 2 = fail
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
int tmInit_OpenFile( String^ filename );

/*===========================================================================================
    Function Name    : tmInit_SaveFile
    Input            : filename: filename
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmInit_SaveFile( String^ filename );

/*===========================================================================================
    Function Name    : tmInit_GetItem
    Input            : index: index
    Return           : Item in String
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
String^ tmInit_GetItem( int index );

/*===========================================================================================
    Function Name    : tmInit_SetItem
    Input            :	text
						index
    Return           : NULL
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmInit_SetItem( String^ text, int index );

#endif

/************************** <END OF FILE> *****************************************/



