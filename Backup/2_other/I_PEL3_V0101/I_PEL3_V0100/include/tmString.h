/*===========================================================================================

    File Name       : tmString.h

    Version         : V1_00_a

    Built Date      : 2016/10/18

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2016/10/18 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_STRING_H
#define TM_STRING_H
#include "windows.h"
#include <process.h>
#include "String.h"
#include <sstream>
#include <vector>
#include "vcclr.h" 
  
using namespace std;  
using namespace System;  
using namespace System::Runtime::InteropServices; 



using namespace System;
using namespace IO;
using namespace std;


enum{
	CSV_STATE_WAIT4_START = 0,
	CSV_STATE_WAIT4_DOUBLE_QUOTES_END,
	CSV_STATE_WAIT4_NORMAL_END,
	CSV_STATE_NUM
};

/*===========================================================================================
    Function Name    : MarshalString
    Input            : s : input ^String
                       os : output std string
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : String to std string
//==========================================================================================*/
static void MarshalString ( String ^ s, string& os ) {
			using namespace Runtime::InteropServices;
			const char* chars = 
			(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
			os = chars;
			Marshal::FreeHGlobal(IntPtr((void*)chars));
		}


/*===========================================================================================
    Function Name    : tmString2Wstring
    Input            : s : input ^String
    Return           : Output wstring
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : String to wstring
//==========================================================================================*/
static wstring tmString2Wstring ( String ^ s )
{
	pin_ptr<const wchar_t> wch = PtrToStringChars( s );
	wstring basicstring(wch);
	return basicstring;
}


/*===========================================================================================
    Function Name    : wstring tmStdstring2Wstring
    Input            : s : input std string
    Return           : Output wstring
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : std string to wstring
//==========================================================================================*/
static wstring tmStdstring2Wstring( const std::string &s )
{	
    wstring wsTmp( s.begin(), s.end() );
    return wsTmp;
}




/*===========================================================================================
    Function Name    : Number2String
    Input            :	number in int
						dummy_zero: dummy_zero = 0 leads to  1 = 1,
									dummy_zero = 1 leads to  1 = 1,
									dummy_zero = 2 leads to  1 = 01,
									dummy_zero = 3 leads to  1 = 001,
									dummy_zero = 4 leads to  1 = 0001,
    Return           : number in string
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : number to 4 char length string 
//==========================================================================================*/
static String^ Number2String( int number, int dummy_zero = 0 )
{
	String^ result ="";

	int tmp_value = number;


	for( int i = 0; i < dummy_zero - 1; i++ ){			

		if( tmp_value >= pow( 10.0, dummy_zero - 1 ) )
			break;
		else
			result += "0";

		tmp_value = tmp_value * 10;

	}

	result += Convert::ToString( number );

	return result;
}

/*===========================================================================================
    Function Name    : Number2string
    Input            :	number in int
						dummy_zero: dummy_zero = 0 leads to  1 = 1,
									dummy_zero = 1 leads to  1 = 1,
									dummy_zero = 2 leads to  1 = 01,
									dummy_zero = 3 leads to  1 = 001,
									dummy_zero = 4 leads to  1 = 0001,
    Return           : number in string
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : number to 4 char length string 
//==========================================================================================*/
static string Number2string( int number, int dummy_zero = 0 )
{
	string result ="";

	int tmp_value = number;


	for( int i = 0; i < dummy_zero - 1; i++ ){			

		if( tmp_value >= pow( 10.0, dummy_zero - 1 ) )
			break;
		else
			result += "0";

		tmp_value = tmp_value * 10;

	}

	result += to_string( (long double)number );

	return result;
}

/*===========================================================================================
    Function Name    : tm_GetWhatItMeans
    Input            : src, input_num
    Return           : what it means
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static String^ tm_GetWhatItMeans( wstring src, int input_num )
{

	int index_start = 0;
	int index_end = 0;
	int length;
	int dummy_1;	// for ','
	int dummy_2;	// for ';'
	wchar_t key_char;
	String^ Result;
	wstring dummy_str;
	int	value;
	int string_length = src.length();

	dummy_1 = src.find_first_of( L',' , index_start );
	dummy_2 = src.find_first_of( L';' , index_start );
	if( dummy_1 != -1 && dummy_2 == -1 ){
		key_char = L',';
	}else if( dummy_1 == -1 && dummy_2 != -1 ){
		key_char = L';';
	}else{ 
		return Convert::ToString( input_num );
	}
	
	while( 1 ){

		index_end = src.find_first_of( L'=', index_start );
		if( index_end == -1 ){
			break;
		}

		dummy_str = src.substr( index_start, index_end - index_start );
		value = stoi( dummy_str.c_str() );

		// to find end
		index_start = index_end + 1;
		index_end = src.find_first_of( key_char, index_start );

		if( index_end == -1 ){ // no any ',' or ';', means it is the last item
			length = string_length - index_start;
			index_end = string_length;
		}else{
			length = index_end - index_start;
		}

		if( value == input_num ){
			dummy_str = src.substr( index_start, length );
			Result = gcnew String( dummy_str.c_str() );
			//Result = Convert::ToString( input_num ) + "(" + Result + ")";
			Result = Convert::ToString( input_num ) + "  " + Result;
			return Result;
		}else{
			index_start = index_end + 1;
			if( index_start >= string_length || value > input_num ){
				break;
			}
		}
	}

	return Convert::ToString( input_num );

}

/*===========================================================================================
    Function Name    : tm_ReadCSV
    Input            :  1. src: input
						2. dst: output
    Return           : 1 = good format, 0 = bad format
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
static int tm_ReadCSV( String^ src, vector<wstring> *dst )
{

	enum{
		CSV_STATE_WAIT4_START = 0,
		CSV_STATE_WAIT4_DOUBLE_QUOTES_END,
		CSV_STATE_WAIT4_NORMAL_END,
		CSV_STATE_NUM
	};

	int i = 0;
	int next_index = 0;
	int state = CSV_STATE_WAIT4_START;
	int length = src->Length;
	wchar_t data;
	wstring dummy_str = L"";
	dst->clear();

	for( i = 0; i < length; i++ ){

		data = src[ i ];
		next_index = i + 1;

		switch( state ){
		case CSV_STATE_WAIT4_START:
			if( data == L'"' ){
				state = CSV_STATE_WAIT4_DOUBLE_QUOTES_END;
			}else if( data == L',' ){
				dst->push_back( dummy_str );
			}else{
				dummy_str += data;

				if( next_index >= length ){
					dst->push_back( dummy_str );
				}else{
					state = CSV_STATE_WAIT4_NORMAL_END;
				}
			}
			break;
		case CSV_STATE_WAIT4_DOUBLE_QUOTES_END:

			if( data == L'"' ){

				if( next_index >= length ){
					dst->push_back( dummy_str );
					dummy_str = L"";
					state = CSV_STATE_WAIT4_START;
				}else if( src[ next_index ] == L'"' ){
					dummy_str += data;
					i = next_index;
				}else if( src[ next_index ] == L',' ){
					dst->push_back( dummy_str );
					i = next_index;
					dummy_str = L"";
					state = CSV_STATE_WAIT4_START;
				}else{
					// Impossible, Wrong format
					return 0;
				}

			}else{
				dummy_str += data;
			}

			break;

		case CSV_STATE_WAIT4_NORMAL_END:
		default:
			if( data == L'"' ){
				// Impossible, Wrong format
				return 0;
			}else if( next_index >= length ){
				dummy_str += data;
				dst->push_back( dummy_str );
				dummy_str = L"";
				state = CSV_STATE_WAIT4_START;
			}else if( data == L',' ){	
				dst->push_back( dummy_str );
				dummy_str = L"";
				state = CSV_STATE_WAIT4_START;
			}else{
				dummy_str += data;
			}

			break;

		}
			
	}
	
	return 1;

}



#endif

/************************** <END OF FILE> *****************************************/



