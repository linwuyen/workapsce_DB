/*
 * CTYPEDEF.h
 *
 *  Created on: 2022¦~3¤ë22¤é
 *      Author: cody_chen
 */

#ifndef CTYPEDEF_H_
#define CTYPEDEF_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memcfg.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#ifndef bool_t
typedef bool bool_t;
#endif

#ifndef true
#define true  ((bool_t)1)
#endif

#ifndef false
#define false ((bool_t)0)
#endif

#define _2PI()                           6.2831853071795f


#define FG_GET(x, src)   ((src & (x)) == (x)) //((src & (x)) == (x))
#define FG_GETn(x, src)  (!FG_GET(x, src)) //!((src & (x)) == (x))
#define FG_AND(x, src)   (src & (x))
#define FG_SET(x, src)   {src |= (x);}
#define FG_RST(x, src)   {src &= ~(x);}
#define FG_SWTO(x, src)  {src = (x);}

#define UPDATE_PARK_BY_C28          1

#define CNV_SPU2UPU(x)   ((float32_t) x * 0.5f + 0.5f)
#define CNV_UPU2SPU(x)   (((float32_t) x - 0.5f) * 2.0f)



#endif /* CTYPEDEF_H_ */
