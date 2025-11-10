/*===========================================================================================

    File Name       : tmPEL.h

    Version         : V1_00_a

    Built Date      : 2018/01/25

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2018/01/25 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_PEL_H
#define TM_PEL_H

#include "tmDriverCom.h"
#include "tmString.h"

#define PEL_MAJOR_MAX	PARAMETER_MAJOR_NUM_CODE_83
#define PEL_MINOR_MAX	PARAMETER_MINOR_NUM_CODE_83
#define	PEL_TYPE_MAX	( PA_SUBITEM_MIN + 1 )


typedef struct{
	int		Data[ PEL_TYPE_MAX ][ PEL_MAJOR_MAX ][ PEL_MINOR_MAX ];
}Struct_PEL;

public ref class Class_PEL{
public :
	int***		Data;

	public:
	Class_PEL(void)
	{
		Data = new int**[ PEL_TYPE_MAX ];
		for( int i = 0; i < PEL_TYPE_MAX; i++ ){
			Data[i] = new int*[ PEL_MAJOR_MAX ];
			for( int j = 0; j < PEL_MAJOR_MAX; j++ ){
				Data[i][j] = new int[ PEL_MINOR_MAX ];
			}
		}
	}
	~Class_PEL(void){

		for( int i = 0; i < PEL_TYPE_MAX; i++ ){
			for( int j = 0; j < PEL_MAJOR_MAX; j++ ){
				delete []Data[i][j];
			}
			delete []Data[i];
		}
		delete []Data;
	}
};


/*===========================================================================================
    Function Name    : tm_OpenPEL
    Input            : 1. pel
					   2. file_name
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static void tm_OpenPEL( Struct_PEL *pel, String ^ file_name )
{
		
	StreamReader^ readfile = gcnew StreamReader( file_name );
	String^ line;
	
	string  std_line;
	int line_num = 0;
	char index[10];
				
	int major_index = 0;
	int minor_index = 0;
					  
	while( line = readfile->ReadLine() ){

		MarshalString( line, std_line );
		stringstream stream( std_line );
						
		if( line_num < PEL_MAJOR_MAX * ( PEL_MINOR_MAX + 1 ) ){
		
			if( line_num % ( PEL_MINOR_MAX + 1 ) != 0 ){
								
				stream >> index;

				major_index = line_num / ( PEL_MINOR_MAX + 1 );
				minor_index = line_num - major_index * ( PEL_MINOR_MAX + 1 ) - 1;

				stream >>  pel->Data[ PA_SUBITEM_VALUE ][ major_index ][ minor_index ];
				stream >>  pel->Data[ PA_SUBITEM_MAX ][ major_index ][ minor_index ];
				stream >>  pel->Data[ PA_SUBITEM_MIN ][ major_index ][ minor_index ];
				stream >>  pel->Data[ PA_SUBITEM_DEFAULT ][ major_index ][ minor_index ];
			}
		}			 				
		line_num++;
	}

	readfile->Close();

}

/*===========================================================================================
    Function Name    : tm_OpenPEL
    Input            : 1. pel
					   2. file_name
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static void tm_OpenPEL( Class_PEL^ pel, String ^ file_name )
{
		
	StreamReader^ readfile = gcnew StreamReader( file_name );
	String^ line;
	
	string  std_line;
	int line_num = 0;
	char index[10];
				
	int major_index = 0;
	int minor_index = 0;
					  
	while( line = readfile->ReadLine() ){

		MarshalString( line, std_line );
		stringstream stream( std_line );
						
		if( line_num < PEL_MAJOR_MAX * ( PEL_MINOR_MAX + 1 ) ){
		
			if( line_num % ( PEL_MINOR_MAX + 1 ) != 0 ){
								
				stream >> index;

				major_index = line_num / ( PEL_MINOR_MAX + 1 );
				minor_index = line_num - major_index * ( PEL_MINOR_MAX + 1 ) - 1;

				stream >>  pel->Data[ PA_SUBITEM_VALUE ][ major_index ][ minor_index ];
				stream >>  pel->Data[ PA_SUBITEM_MAX ][ major_index ][ minor_index ];
				stream >>  pel->Data[ PA_SUBITEM_MIN ][ major_index ][ minor_index ];
				stream >>  pel->Data[ PA_SUBITEM_DEFAULT ][ major_index ][ minor_index ];
			}
		}			 				
		line_num++;
	}

	readfile->Close();

}

/*===========================================================================================
    Function Name    : tm_SavePEL
    Input            : 1. pel
					   2. file_name
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static void tm_SavePEL( Struct_PEL *pel, String ^ file_name )
{

	StreamWriter^ writefile = gcnew StreamWriter( file_name );
	String^ Line;

	for( int i = 0; i < PEL_MAJOR_MAX; i++ ){

		writefile->WriteLine( "	value		max		min		def" );

		for( int j = 0; j < PEL_MINOR_MAX; j++ ){
							 
			Line = ( i < 9 ? "0" + Convert::ToString( i + 1 ) + "-" : Convert::ToString( i + 1 ) + "-" );
			Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

			Line +=	Convert::ToString( pel->Data[ PA_SUBITEM_VALUE ][ i ][ j ] ) + "		" +
					Convert::ToString( pel->Data[ PA_SUBITEM_MAX ][ i ][ j ] ) + "		" +
					Convert::ToString( pel->Data[ PA_SUBITEM_MIN ][ i ][ j ] ) + "		" +
					Convert::ToString( pel->Data[ PA_SUBITEM_DEFAULT ][ i ][ j ] );
			 
			writefile->WriteLine( Line ); 

		}
	}
	
	writefile->Close();

}

/*===========================================================================================
    Function Name    : tm_SavePEL
    Input            : 1. pel
					   2. file_name
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static void tm_SavePEL( Class_PEL^ pel, String ^ file_name )
{

	StreamWriter^ writefile = gcnew StreamWriter( file_name );
	String^ Line;

	for( int i = 0; i < PEL_MAJOR_MAX; i++ ){

		writefile->WriteLine( "	value		max		min		def" );

		for( int j = 0; j < PEL_MINOR_MAX; j++ ){
							 
			Line = ( i < 9 ? "0" + Convert::ToString( i + 1 ) + "-" : Convert::ToString( i + 1 ) + "-" );
			Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

			Line +=	Convert::ToString( pel->Data[ PA_SUBITEM_VALUE ][ i ][ j ] ) + "		" +
					Convert::ToString( pel->Data[ PA_SUBITEM_MAX ][ i ][ j ] ) + "		" +
					Convert::ToString( pel->Data[ PA_SUBITEM_MIN ][ i ][ j ] ) + "		" +
					Convert::ToString( pel->Data[ PA_SUBITEM_DEFAULT ][ i ][ j ] );
			 
			writefile->WriteLine( Line ); 

		}
	}
	
	writefile->Close();

}

#endif

/************************** <END OF FILE> *****************************************/



