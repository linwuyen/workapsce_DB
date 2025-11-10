/*===========================================================================================

    File Name       : tmParameter.h

    Version         : V1_00_a

    Built Date      : 2018/01/03

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2018/01/03 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_PARAMETER_H
#define TM_PARAMETER_H
#include "windows.h"
#include <process.h>
#include "String.h"
#include <sstream>
#include <vector>

using namespace System;
using namespace IO;
using namespace std;

#define PA_LVL_0_TEXT		L"0"
#define PA_LVL_1_TEXT		L"1"
#define PA_LVL_NONDEF_TEXT	L"-"

#define APPLY_NEED_CONFIG_TEXT_1		L"C"
#define APPLY_NEED_CONFIG_TEXT_2		L"c"

#define APPLY_NEED_REPOWER_TEXT_1		L"D"
#define APPLY_NEED_REPOWER_TEXT_2		L"d"


// ==================================================================================

enum{
	PA_ID		= 0,
	PA_VALUE,
	PA_DEF,
	PA_MAX,
	PA_MIN,
	PA_RAM,
	PA_ADDRESS,
	PA_LEVEL,
	PA_TYPE,
	PA_NAME,
	PA_TIP,
	PA_APPLY,
	PA_VERSION,
	PA_DOCUMENT,

	PA_GROUP,
	PA_NUM

};

typedef struct{

	wstring	Items[ PA_NUM ];

}Struct_Parameter;

#endif

/************************** <END OF FILE> *****************************************/



