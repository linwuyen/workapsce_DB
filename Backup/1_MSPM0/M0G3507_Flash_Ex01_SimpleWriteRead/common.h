/*
 * common.h
 *
 *  Created on: Mar 13, 2024
 *      Author: cody_chen
 */

#ifndef COMMON_H_
#define COMMON_H_

#define FG_GET(x, src)   ((src & (x)) == (x)) //((src & (x)) == (x))
#define FG_GETn(x, src)  (!FG_GET(x, src)) //!((src & (x)) == (x))
#define FG_AND(x, src)   (src & (x))
#define FG_SET(x, src)   {src |= (x);}
#define FG_RST(x, src)   {src &= ~(x);}



#endif /* COMMON_H_ */
