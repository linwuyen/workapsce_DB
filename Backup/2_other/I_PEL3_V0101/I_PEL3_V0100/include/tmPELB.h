/*===========================================================================================

    File Name       : tmPELB.h

    Version         : V1_00_a

    Built Date      : 2020/06/19

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2018/01/25 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_PELB_H
#define TM_PELB_H

#include "tmDriverCom.h"
#include "tmString.h"

#define PELB_MAJOR_MAX	PARAMETER_MAJOR_NUM_CODE_86
#define PELB_MINOR_MAX	PARAMETER_MINOR_NUM_CODE_86
#define	PELB_TYPE_MAX		( PA_SUBITEM_RAM + 1 )


typedef struct{
	int		Data[ PELB_TYPE_MAX ][ PELB_MAJOR_MAX ][ PELB_MINOR_MAX ];
	int		AlarmHistory[ PELB_MINOR_MAX ];
	int		WarnningHistory[ PELB_MINOR_MAX ];
	int		ComErrorHistory[ PELB_MINOR_MAX ];

}Struct_PELB;

public ref class Class_PELB{
public :
	int***		Data;
	int*		AlarmHistory;
	int*		WarnningHistory;
	int*		ComErrorHistory;

	public:
	Class_PELB(void)
	{
		Data = new int**[ PELB_TYPE_MAX ];
		for( int i = 0; i < PELB_TYPE_MAX; i++ ){
			Data[i] = new int*[ PELB_MAJOR_MAX ];
			for( int j = 0; j < PELB_MAJOR_MAX; j++ ){
				Data[i][j] = new int[ PELB_MINOR_MAX ];
			}
		}
		AlarmHistory = new int[ PELB_MINOR_MAX ];
		WarnningHistory = new int[ PELB_MINOR_MAX ];
		ComErrorHistory = new int[ PELB_MINOR_MAX ];
	}
	~Class_PELB(void){

		for( int i = 0; i < PELB_TYPE_MAX; i++ ){
			for( int j = 0; j < PELB_MAJOR_MAX; j++ ){
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
    Function Name    : tm_OpenPELB
    Input            : 1. pelb
					   2. file_name
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static void tm_OpenPELB( Struct_PELB *pelb, String ^ file_name )
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
						
		if( line_num < PELB_MAJOR_MAX * ( PELB_MINOR_MAX + 1 ) ){
		
			if( line_num % ( PELB_MINOR_MAX + 1 ) != 0 ){
								
				stream >> index;

				major_index = line_num / ( PELB_MINOR_MAX + 1 );
				minor_index = line_num - major_index * ( PELB_MINOR_MAX + 1 ) - 1;

				stream >>  pelb->Data[ PA_SUBITEM_VALUE ][ major_index ][ minor_index ];
				stream >>  pelb->Data[ PA_SUBITEM_DEFAULT ][ major_index ][ minor_index ];
				stream >>  pelb->Data[ PA_SUBITEM_MAX ][ major_index ][ minor_index ];
				stream >>  pelb->Data[ PA_SUBITEM_MIN ][ major_index ][ minor_index ];
				stream >>  pelb->Data[ PA_SUBITEM_RAM ][ major_index ][ minor_index ];

			}
		}			 				
		line_num++;
	}

	readfile->Close();

}

/*===========================================================================================
    Function Name    : tm_OpenPELB
    Input            : 1. pelb
					   2. file_name
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static void tm_OpenPELB( Class_PELB^ pelb, String ^ file_name )
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
						
		if( line_num < PELB_MAJOR_MAX * ( PELB_MINOR_MAX + 1 ) ){
		
			if( line_num % ( PELB_MINOR_MAX + 1 ) != 0 ){
								
				stream >> index;

				major_index = line_num / ( PELB_MINOR_MAX + 1 );
				minor_index = line_num - major_index * ( PELB_MINOR_MAX + 1 ) - 1;

				stream >>  pelb->Data[ PA_SUBITEM_VALUE ][ major_index ][ minor_index ];
				stream >>  pelb->Data[ PA_SUBITEM_DEFAULT ][ major_index ][ minor_index ];
				stream >>  pelb->Data[ PA_SUBITEM_MAX ][ major_index ][ minor_index ];
				stream >>  pelb->Data[ PA_SUBITEM_MIN ][ major_index ][ minor_index ];
				stream >>  pelb->Data[ PA_SUBITEM_RAM ][ major_index ][ minor_index ];

			}
		}			 				
		line_num++;
	}

	readfile->Close();

}

/*===========================================================================================
    Function Name    : tm_SavePELB
    Input            : 1. pelb
					   2. file_name
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static void tm_SavePELB( Struct_PELB *pelb, String ^ file_name )
{

	StreamWriter^ writefile = gcnew StreamWriter( file_name );
	String^ Line;

	for( int i = 0; i < PELB_MAJOR_MAX; i++ ){

		writefile->WriteLine( "	value		def		max		min		Ram" );

		for( int j = 0; j < PELB_MINOR_MAX; j++ ){
							 
			Line = ( i < 9 ? "0" + Convert::ToString( i + 1 ) + "-" : Convert::ToString( i + 1 ) + "-" );
			Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

			Line +=	Convert::ToString( pelb->Data[ PA_SUBITEM_VALUE ][ i ][ j ] ) + "		" +
					Convert::ToString( pelb->Data[ PA_SUBITEM_DEFAULT ][ i ][ j ] ) + "		" +
					Convert::ToString( pelb->Data[ PA_SUBITEM_MAX ][ i ][ j ] ) + "		" +
					Convert::ToString( pelb->Data[ PA_SUBITEM_MIN ][ i ][ j ] ) + "		" +
					Convert::ToString( pelb->Data[ PA_SUBITEM_RAM ][ i ][ j ] );
			 
			writefile->WriteLine( Line ); 

		}
	}
	// Alarm history
	writefile->WriteLine( "	value		def		max		min		Ram" );
	for( int j = 0; j < PELB_MINOR_MAX; j++ ){
		Line = "10-";
		Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

		Line +=	Convert::ToString( pelb->AlarmHistory[ j ] ) + "		" +
				"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				"0";
			 
		writefile->WriteLine( Line );
	}
	// Warnning history
	writefile->WriteLine( "	value		def		max		min		Ram" );
	for( int j = 0; j < PELB_MINOR_MAX; j++ ){
		Line = "11-";
		Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

		Line +=	"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				Convert::ToString( pelb->WarnningHistory[ j ] );
			 
		writefile->WriteLine( Line );
	}
	// ComError history
	writefile->WriteLine( "	value		def		max		min		Ram" );
	for( int j = 0; j < PELB_MINOR_MAX; j++ ){
		Line = "12-";
		Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

		Line +=	"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				Convert::ToString( pelb->ComErrorHistory[ j ] );
			 
		writefile->WriteLine( Line );
	}

	writefile->Close();

}

/*===========================================================================================
    Function Name    : tm_SavePELB
    Input            : 1. pelb
					   2. file_name
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static void tm_SavePELB( Class_PELB^ pelb, String ^ file_name )
{

	StreamWriter^ writefile = gcnew StreamWriter( file_name );
	String^ Line;

	for( int i = 0; i < PELB_MAJOR_MAX; i++ ){

		writefile->WriteLine( "	value		def		max		min		Ram" );

		for( int j = 0; j < PELB_MINOR_MAX; j++ ){
							 
			Line = ( i < 9 ? "0" + Convert::ToString( i + 1 ) + "-" : Convert::ToString( i + 1 ) + "-" );
			Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

			Line +=	Convert::ToString( pelb->Data[ PA_SUBITEM_VALUE ][ i ][ j ] ) + "		" +
					Convert::ToString( pelb->Data[ PA_SUBITEM_DEFAULT ][ i ][ j ] ) + "		" +
					Convert::ToString( pelb->Data[ PA_SUBITEM_MAX ][ i ][ j ] ) + "		" +
					Convert::ToString( pelb->Data[ PA_SUBITEM_MIN ][ i ][ j ] ) + "		" +
					Convert::ToString( pelb->Data[ PA_SUBITEM_RAM ][ i ][ j ] );
			 
			writefile->WriteLine( Line ); 

		}
	}
	// Alarm history
	writefile->WriteLine( "	value		def		max		min		Ram" );
	for( int j = 0; j < PELB_MINOR_MAX; j++ ){
		Line = "10-";
		Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

		Line +=	Convert::ToString( pelb->AlarmHistory[ j ] ) + "		" +
				"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				"0";
			 
		writefile->WriteLine( Line );
	}
	// Warnning history
	writefile->WriteLine( "	value		def		max		min		Ram" );
	for( int j = 0; j < PELB_MINOR_MAX; j++ ){
		Line = "11-";
		Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

		Line +=	"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				Convert::ToString( pelb->WarnningHistory[ j ] );
			 
		writefile->WriteLine( Line );
	}
	// ComError history
	writefile->WriteLine( "	value		def		max		min		Ram" );
	for( int j = 0; j < PELB_MINOR_MAX; j++ ){
		Line = "12-";
		Line += ( j < 9 ? "0" + Convert::ToString( j + 1 ) + ":	" : Convert::ToString( j + 1 ) + ":	" );

		Line +=	"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				"0" + "		" +
				Convert::ToString( pelb->ComErrorHistory[ j ] );
			 
		writefile->WriteLine( Line );
	}

	writefile->Close();

}
#endif

/************************** <END OF FILE> *****************************************/



