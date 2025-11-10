/*===========================================================================================
    File Name       : Type.h
    Version         : V1.02a
    Built Date      : 2012-10-24
    Release Date    : Not Yet
    Programmer      : Chaim.Chen@trumman.com.tw
    Description     : This file provide type defines for C language.
    =========================================================================================
    History         : 2012-10-24 first released version
					  2014-07-22 V1.01r(R8): Declaration format modified.
					  2014-08-01 V1.02a(R8): Binary expression macro added.
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */


#ifndef TYPE_H
#define TYPE_H


#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

#ifndef HIGH
#define HIGH    (1)
#endif

#ifndef LOW
#define LOW     (0)
#endif

#ifndef NO
#define NO		(0)
#endif

#ifndef YES
#define YES !NO
#endif

#ifndef FINE
#define FINE	(1)
#endif

#ifndef BAD
#define BAD		(0)
#endif


/*===========================================================================================
    Function Name    : tmInterpolation
    Input            : ( x1, y1 ), ( x2, y2 ) and X_in
    Return           : y
    Programmer       : Chaum.Chen@trumman.com.tw
    Description      : Use interpolation method to get y of x by ( x1, y1 ) and ( x2, y2 )
//==========================================================================================*/
static int tmInterpolation( const int x1, const int y1, const int x2, const int y2, int X_in )
{
	if( x1 - x2 != 0 ){
		return ( ( y1 - y2 ) * X_in + x1 * y2 -x2 * y1 ) / ( x1 - x2 );
	}else{
		return 0;
	}
}



#endif
