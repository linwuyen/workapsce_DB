/*===========================================================================================

    File Name       : tmPEL2.h

    Version         : V1_00_a

    Built Date      : 2018/01/25

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2018/01/25 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_PEL2_H
#define TM_PEL2_H

#include "tmDriverCom.h"
#include "tmString.h"

#define PEL2_MAJOR_MAX	PARAMETER_MAJOR_NUM_CODE_85
#define PEL2_MINOR_MAX	PARAMETER_MINOR_NUM_CODE_85
#define	PEL2_TYPE_MAX		( PA_SUBITEM_RAM + 1 )


typedef struct{
	int		Data[ PEL2_TYPE_MAX ][ PEL2_MAJOR_MAX ][ PEL2_MINOR_MAX ];
	int		AlarmHistory[ PEL2_MINOR_MAX ];
	int		WarnningHistory[ PEL2_MINOR_MAX ];
	int		ComErrorHistory[ PEL2_MINOR_MAX ];

}Struct_PEL2;

public ref class Class_PEL2{
public :
	int***		Data;
	int*		AlarmHistory;
	int*		WarnningHistory;
	int*		ComErrorHistory;

	public:
	Class_PEL2(void)
	{
		Data = new int**[ PEL2_TYPE_MAX ];
		for( int i = 0; i < PEL2_TYPE_MAX; i++ ){
			Data[i] = new int*[ PEL2_MAJOR_MAX ];
			for( int j = 0; j < PEL2_MAJOR_MAX; j++ ){
				Data[i][j] = new int[ PEL2_MINOR_MAX ];
			}
		}
		AlarmHistory = new int[ PEL2_MINOR_MAX ];
		WarnningHistory = new int[ PEL2_MINOR_MAX ];
		ComErrorHistory = new int[ PEL2_MINOR_MAX ];
	}
	~Class_PEL2(void){

		for( int i = 0; i < PEL2_TYPE_MAX; i++ ){
			for( int j = 0; j < PEL2_MAJOR_MAX; j++ ){
				delete []Data[i][j];
			}
			delete []Data[i];
		}
		delete []Data;
		delete []AlarmHistory;
		delete []WarnningHistory;
		delete []ComErrorHistory;

	}

};


/*===========================================================================================
    Function Name    : tm_OpenPEL2
    Input            : 1. pel2
					   2. file_name
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static void tm_OpenPEL2( Struct_PEL2 *pel2, String ^ file_name )
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
						
		if( line_num < PEL2_MAJOR_MAX * ( PEL2_MINOR_MAX + 1 ) ){
		
			if( line_num % ( PEL2_MINOR_MAX + 1 ) != 0 ){
								
				stream >> index;

				major_index = line_num / ( PEL2_MINOR_MAX + 1 );
				minor_index = line_num - major_index * ( PEL2_MINOR_MAX + 1 ) - 1;

				stream >>  pel2->Data[ PA_SUBITEM_VALUE ][ major_index ][ minor_index ];
				stream >>  pel2->Data[ PA_SUBITEM_DEFAULT ][ major_index ][ minor_index ];
				stream >>  pel2->Data[ PA_SUBITEM_MAX ][ major_index ][ minor_index ];
				stream >>  pel2->Data[ PA_SUBITEM_MIN ][ major_index ][ minor_index ];
				stream >>  pel2->Data[ PA_SUBITEM_RAM ][ major_index ][ minor_index ];

			}
		}			 				
		line_num++;
	}

	readfile->Close();

}

/*===========================================================================================
    Function Name    : tm_OpenPEL2
    Input            : 1. pel2
					   2. file_name
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static void tm_OpenPEL2( Class_PEL2^ pel2, String ^ file_name )
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
						
		if( line_num < PEL2_MAJOR_MAX * ( PEL2_MINOR_MAX + 1 ) ){
		
			if( line_num % ( PEL2_MINOR_MAX + 1 ) != 0 ){
								
				stream >> index;

				major_index = line_num / ( PEL2_MINOR_MAX + 1 );
				minor_index = line_num - major_index * ( PEL2_MINOR_MAX + 1 ) - 1;

				stream >>  pel2->Data[ PA_SUBITEM_VALUE ][ major_index ][ minor_index ];
				stream >>  pel2->Data[ PA_SUBITEM_DEFAULT ][ major_index ][ minor_index ];
				stream >>  pel2->Data[ PA_SUBITEM_MAX ][ major_index ][ minor_index ];
				stream >>  pel2->Data[ PA_SUBITEM_MIN ][ major_index ][ minor_index ];
				stream >>  pel2->Data[ PA_SUBITEM_RAM ][ major_index ][ minor_index ];

			}
		}			 				
		line_num++;
	}

	readfile->Close();

}

/*===========================================================================================
    Function Name    : tm_SavePEL2
    Input            : 1. pel2
					   2. file_name
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static void tm_SavePEL2( Struct_PEL2 *pel2, String ^ file_name )
{

	StreamWriter^ writefile = gcnew StreamWriter( file_name );
	String^ Line;

	for( int i = 0; i < PEL2_MAJOR_MAX; i++ ){

		writefile->WriteLine( "	value		def		max		min		Ram" );

		for( int j = 0; j < PEL2_MINOR_MAX; j++ ){
							 
			Line = ( i < 9 ? "0" + Convert::ToString( i + 1 ) + "-" : Convert::ToString( i + 1 ) + "-" );
			Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

			Line +=	Convert::ToString( pel2->Data[ PA_SUBITEM_VALUE ][ i ][ j ] ) + "		" +
					Convert::ToString( pel2->Data[ PA_SUBITEM_DEFAULT ][ i ][ j ] ) + "		" +
					Convert::ToString( pel2->Data[ PA_SUBITEM_MAX ][ i ][ j ] ) + "		" +
					Convert::ToString( pel2->Data[ PA_SUBITEM_MIN ][ i ][ j ] ) + "		" +
					Convert::ToString( pel2->Data[ PA_SUBITEM_RAM ][ i ][ j ] );
			 
			writefile->WriteLine( Line ); 

		}
	}
	// Alarm history
	writefile->WriteLine( "	value		def		max		min		Ram" );
	for( int j = 0; j < PEL2_MINOR_MAX; j++ ){
		Line = "10-";
		Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

		Line +=	Convert::ToString( pel2->AlarmHistory[ j ] ) + "		" +
				"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				"0";
			 
		writefile->WriteLine( Line );
	}
	// Warnning history
	writefile->WriteLine( "	value		def		max		min		Ram" );
	for( int j = 0; j < PEL2_MINOR_MAX; j++ ){
		Line = "11-";
		Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

		Line +=	"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				Convert::ToString( pel2->WarnningHistory[ j ] );
			 
		writefile->WriteLine( Line );
	}
	// ComError history
	writefile->WriteLine( "	value		def		max		min		Ram" );
	for( int j = 0; j < PEL2_MINOR_MAX; j++ ){
		Line = "12-";
		Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

		Line +=	"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				Convert::ToString( pel2->ComErrorHistory[ j ] );
			 
		writefile->WriteLine( Line );
	}

	writefile->Close();

}

/*===========================================================================================
    Function Name    : tm_SavePEL2
    Input            : 1. pel2
					   2. file_name
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static void tm_SavePEL2( Class_PEL2^ pel2, String ^ file_name )
{

	StreamWriter^ writefile = gcnew StreamWriter( file_name );
	String^ Line;

	for( int i = 0; i < PEL2_MAJOR_MAX; i++ ){

		writefile->WriteLine( "	value		def		max		min		Ram" );

		for( int j = 0; j < PEL2_MINOR_MAX; j++ ){
							 
			Line = ( i < 9 ? "0" + Convert::ToString( i + 1 ) + "-" : Convert::ToString( i + 1 ) + "-" );
			Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

			Line +=	Convert::ToString( pel2->Data[ PA_SUBITEM_VALUE ][ i ][ j ] ) + "		" +
					Convert::ToString( pel2->Data[ PA_SUBITEM_DEFAULT ][ i ][ j ] ) + "		" +
					Convert::ToString( pel2->Data[ PA_SUBITEM_MAX ][ i ][ j ] ) + "		" +
					Convert::ToString( pel2->Data[ PA_SUBITEM_MIN ][ i ][ j ] ) + "		" +
					Convert::ToString( pel2->Data[ PA_SUBITEM_RAM ][ i ][ j ] );
			 
			writefile->WriteLine( Line ); 

		}
	}
	// Alarm history
	writefile->WriteLine( "	value		def		max		min		Ram" );
	for( int j = 0; j < PEL2_MINOR_MAX; j++ ){
		Line = "10-";
		Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

		Line +=	Convert::ToString( pel2->AlarmHistory[ j ] ) + "		" +
				"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				"0";
			 
		writefile->WriteLine( Line );
	}
	// Warnning history
	writefile->WriteLine( "	value		def		max		min		Ram" );
	for( int j = 0; j < PEL2_MINOR_MAX; j++ ){
		Line = "11-";
		Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

		Line +=	"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				Convert::ToString( pel2->WarnningHistory[ j ] );
			 
		writefile->WriteLine( Line );
	}
	// ComError history
	writefile->WriteLine( "	value		def		max		min		Ram" );
	for( int j = 0; j < PEL2_MINOR_MAX; j++ ){
		Line = "12-";
		Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

		Line +=	"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				Convert::ToString( pel2->ComErrorHistory[ j ] );
			 
		writefile->WriteLine( Line );
	}

	writefile->Close();

}
#endif

/************************** <END OF FILE> *****************************************/



