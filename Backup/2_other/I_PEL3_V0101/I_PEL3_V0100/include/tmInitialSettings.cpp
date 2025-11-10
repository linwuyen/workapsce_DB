/*===========================================================================================

    File Name       : tmFileAccess.cpp
					  tmInitialSettings.cpp  ( 2018/01/04 Modified name from tmFileAccess.cpp )

    Version         : V1_00_a

    Built Date      : 2016/10/21

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2016/10/21 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */
#include "stdafx.h"
#include "windows.h"
#include <process.h>
#include "String.h"
#include <sstream>
#include <vector>
#include "tmString.h"
#include "tmInitialSettings.h"


using namespace System;
using namespace IO;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

Struct_Initial	CG_Init;



/*===========================================================================================
    Function Name    : tmInit_OpenFile
    Input            : filename: filename
    Return           : 1 = ok, 2 = fail
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
int tmInit_OpenFile( String^ filename )
{  
    int i;
	StreamReader^ readfile;

    try {
		readfile = gcnew StreamReader( filename );
	}catch(...){
		return 0;
	}

    String ^line;
 
	for( i = 0; i < INIT_NUM; i++ ){
         line = readfile->ReadLine();
		 MarshalString ( line, CG_Init.Items[i] ); 
	}

    readfile->Close();

	return 1;
}

/*===========================================================================================
    Function Name    : tmInit_SaveFile
    Input            : filename: filename
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmInit_SaveFile( String^ filename )
{  
	
    int i;
    StreamWriter^ writefile = gcnew StreamWriter( filename );
    
	for( i = 0; i < INIT_NUM; i++ ){
		String^ dummy_string = gcnew String( CG_Init.Items[ i ].c_str() );
        writefile->WriteLine( dummy_string );
    }
	
    writefile->Close();
	
}

/*===========================================================================================
    Function Name    : tmInit_GetItem
    Input            : index: index
    Return           : Item in String
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
String^ tmInit_GetItem( int index )
{
	String^ Result;

    if( index < INIT_NUM ){
        Result = gcnew String( CG_Init.Items[ index ].c_str() );
    }else{
        Result = gcnew String( "" );
    }

	return Result;
}


/*===========================================================================================
    Function Name    : tmInit_SetItem
    Input            :	text
						index
    Return           : NULL
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmInit_SetItem( String^ text, int index )
{
     MarshalString ( text, CG_Init.Items[ index ] );
}


/************************** <END OF FILE> *****************************************/




