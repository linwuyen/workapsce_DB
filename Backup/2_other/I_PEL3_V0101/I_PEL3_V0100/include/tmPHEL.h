/*===========================================================================================

    File Name       : tmPHEL.h

    Version         : V1_00_a

    Built Date      : 2019/12/25

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2019/012/25 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_PHEL_H
#define TM_PHEL_H

#include "tmDriverCom.h"
#include "tmString.h"
#include <vector>

using namespace std;

#define HW_DATA_SIZE		256

enum{
	PHEL_DATA_TYPE_1BYTE_NUMBER				= 0,
	PHEL_DATA_TYPE_1BYTE_CHAR,
	PHEL_DATA_TYPE_2BYTE_NUMBER,
	PHEL_DATA_TYPE_NUM
};


typedef struct{

	int	Type;
	int	Address_H;
	int	Address_L;
	int	Num;
	int Index;

}Struct_PHEL_Item;

typedef struct{
	int	Data[ HW_DATA_SIZE ];
	int	DummyData[ HW_DATA_SIZE ];

	int	ReadBack_Value[ HW_DATA_SIZE ];
	char ReadBack_Char[ HW_DATA_SIZE ];

	vector<Struct_PHEL_Item> Items;

}Struct_PHEL;

public ref class Class_PHEL{
public :
	int*			Data;
	int*			DummyData;
	int*			ReadBack_Value;
	char*			ReadBack_Char;
	vector<Struct_PHEL_Item> *Items;

	public:
	Class_PHEL(void)
	{
		Data = new int[ HW_DATA_SIZE ];
		DummyData = new int[ HW_DATA_SIZE ];
		ReadBack_Value = new int[ HW_DATA_SIZE ];
		ReadBack_Char = new char[ HW_DATA_SIZE ];

		Items = new vector<Struct_PHEL_Item>;
	}
	~Class_PHEL(void){
		delete []Data;
		delete []DummyData;
		delete []ReadBack_Value;
		delete []ReadBack_Char;
		delete Items;
	}

};


/*===========================================================================================
    Function Name    : tm_DisplayPHELItem
    Input            : 1. item
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static string tm_DisplayPHELItem( Struct_PHEL *phel,Struct_PHEL_Item *item )
{
	string display_str = "";
	string dummy_str;
	char *dummy;

	switch( item->Type ){

	case PHEL_DATA_TYPE_1BYTE_NUMBER:	//	1 byte num
		
		display_str = "Type = 1Byte Num, Address = " +
						to_string( (long double)item->Address_H ) +
						", " +
						to_string( (long double)item->Address_L ) +
						", Num = " +
						to_string( (long double)item->Num ) +
						", Data = ";

		for( int i = 0; i < item->Num; i++ ){
			display_str += to_string( (long double)phel->Data[ item->Index + i ] );
			if( i != item->Num - 1 ){
				display_str += " ";
			}
		}

		break;
	case PHEL_DATA_TYPE_1BYTE_CHAR: //	1 byte char
		display_str = "Type = 1Byte Char, Address = " +
						to_string( (long double)item->Address_H ) +
						", " +
						to_string( (long double)item->Address_L ) +
						", Num = " +
						to_string( (long double)item->Num ) +
						", Data = ";

		dummy = new char[ item->Num + 1 ];
		for( int i = 0; i < item->Num; i++ ){
			dummy[i] = phel->Data[ item->Index + i ];
		}
		dummy[ item->Num ] = '\0';
		dummy_str = dummy;
		delete []dummy;

		display_str += dummy_str;

		break;
	case PHEL_DATA_TYPE_2BYTE_NUMBER:	//	2 byte num

		display_str = "Type = 2Byte Num, Address = " +
						to_string( (long double)item->Address_H ) +
						", " +
						to_string( (long double)item->Address_L ) +
						", Num = " +
						to_string( (long double)item->Num ) +
						", Data = ";

		for( int i = 0; i < item->Num; i++ ){
			display_str += to_string( (long double)( phel->Data[ item->Index + i * 2 + 0 ] * 256 + phel->Data[ item->Index + i * 2 + 1 ] ) );
			if( i != item->Num - 1 ){
				display_str += " ";
			}
		}

		break;
	default:// no defined
		display_str = "Type = " + to_string( (long double)item->Type ) + "¤£¤ä´©";

		break;
	}
	return display_str;
}

/*===========================================================================================
    Function Name    : tm_OpenPHEL
    Input            : 1. phel
					   2. file_name
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static void tm_OpenPHEL( Struct_PHEL *phel, String ^ file_name )
{
		
	StreamReader^ readfile = gcnew StreamReader( file_name );
	String^ line;	
	string  std_line;
	string  dummy_str;
	int dummy_int;

	// Clear data
	for( int i = 0; i < HW_DATA_SIZE; i++ ){
		phel->Data[i] = 0;
	}
	phel->Items.clear();

	// Data fill in				  
	while( line = readfile->ReadLine() ){

		MarshalString( line, std_line );
		stringstream stream( std_line );

		Struct_PHEL_Item item;

		stream >> dummy_str;
		if( dummy_str != "Block" && dummy_str != "Des" ){

			item.Index = atoi( dummy_str.c_str() );
			stream >> item.Num;
			stream >> item.Type;
			stream >> item.Address_H;
			stream >> item.Address_L;

			phel->Items.push_back( item );

			switch( item.Type ){

			case 0:	//	1 byte num

				for( int i = 0; i < item.Num; i++ ){
					stream >> phel->Data[ item.Index + i ];
				}

				break;
			case 1: //	1 byte char
				stream >> dummy_str;
				for( int i = 0; i < item.Num; i++ ){
					if( i < (int)dummy_str.size() ){
						phel->Data[ item.Index + i ] = dummy_str[ i ];
					}else{
						phel->Data[ item.Index + i ] = ' ';
					}
				}

				break;
			case 2:	//	2 byte num

				for( int i = 0; i < item.Num; i++ ){
					stream >> dummy_int;
					phel->Data[ item.Index + i * 2 + 0 ] = dummy_int / 256;
					phel->Data[ item.Index + i * 2 + 1 ] = dummy_int % 256;
				}

				break;
			default:// no defined
				break;
			}
					
		}

	}

	readfile->Close();

}


/*===========================================================================================
    Function Name    : tm_OpenPHEL
    Input            : 1. phel
					   2. file_name
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static void tm_OpenPHEL( Class_PHEL^ phel, String ^ file_name )
{
		
	StreamReader^ readfile = gcnew StreamReader( file_name );
	String^ line;	
	string  std_line;
	string  dummy_str;
	int dummy_int;

	// Clear data
	for( int i = 0; i < HW_DATA_SIZE; i++ ){
		phel->Data[i] = 0;
	}
	phel->Items->clear();

	// Data fill in				  
	while( line = readfile->ReadLine() ){

		MarshalString( line, std_line );
		stringstream stream( std_line );

		Struct_PHEL_Item item;

		stream >> dummy_str;
		if( dummy_str != "Block" && dummy_str != "Des" ){

			item.Index = atoi( dummy_str.c_str() );
			stream >> item.Num;
			stream >> item.Type;
			stream >> item.Address_H;
			stream >> item.Address_L;

			phel->Items->push_back( item );

			switch( item.Type ){

			case 0:	//	1 byte num

				for( int i = 0; i < item.Num; i++ ){
					stream >> phel->Data[ item.Index + i ];
				}

				break;
			case 1: //	1 byte char
				stream >> dummy_str;
				for( int i = 0; i < item.Num; i++ ){
					if( i < (int)dummy_str.size() ){
						phel->Data[ item.Index + i ] = dummy_str[ i ];
					}else{
						phel->Data[ item.Index + i ] = ' ';
					}
				}

				break;
			case 2:	//	2 byte num

				for( int i = 0; i < item.Num; i++ ){
					stream >> dummy_int;
					phel->Data[ item.Index + i * 2 + 0 ] = dummy_int / 256;
					phel->Data[ item.Index + i * 2 + 1 ] = dummy_int % 256;
				}

				break;
			default:// no defined
				break;
			}
					
		}

	}

	readfile->Close();

}


#endif


/************************** <END OF FILE> *****************************************/



