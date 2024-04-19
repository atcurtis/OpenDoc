/* @(#)Z 1.5 com/src/cm/Utility.c, odstorage, od96os2, odos29646d 96/11/15 15:27:39 (96/10/29 09:20:11) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
 *
 *   CLASSES: none
 *
 *   ORIGINS: 82,27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1995,1996
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *   	
 *   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 *   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 *   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
 *   OR PERFORMANCE OF THIS SOFTWARE.
 */
/*====END_GENERATED_PROLOG========================================
 */

/*
	File:		Utility.c

	Contains:	Container Manager Miscellaneous Utility Routines

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <1>	  2/3/94	EL		first checked in

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                         <<<     Utility.c     >>>                         |
 |                                                                           |
 |             Container Manager Miscellaneous Utility Routines              |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/02/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This file containes a collection of generally usefull utility routines that don't
 logically fit anywhere else.
*/


#include <stddef.h>
#include <string.h>
#include <stdio.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif
#ifndef __TOCENTRIES__
#include "TOCEnts.h"   
#endif
#ifndef __TOCOBJECTS__
#include "TOCObjs.h"   
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __UTILITYROUTINES__
#include "Utility.h"        
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif

																	CM_CFUNCTIONS

/* The following generates a segment directive for Mac only due to 32K Jump Table 			*/
/* Limitations.  If you don't know what I'm talking about don't worry about it.  The		*/
/* default is not to generate the pragma.  Theoritically unknown pragmas in ANSI won't	*/
/* choke compilers that don't recognize them.  Besides why would they be looked at if		*/
/* it's being conditionally skipped over anyway?  I know, famous last words!						*/

#if CM_MPW
#pragma segment UtilityRoutines
#endif


/*-----------------------------------------------*
 | cmltostr - convert a long integer to a string |
 *-----------------------------------------------*

 Converts the (long) integer n to a string. If width>0 and width > strlen(string), then
 the number will be right-justified in a string of width characters.  If width<0 and 
 abs(width) > length(string), then the number is again right-justified but padded with
 leading zeros instead of blanks.	In this case a negative number will have a minus as
 its first character.  If width is too small the entire number is returned overflowing
 the width.
 
 If hex conversion is desired, set the hexConversion parameter to true.
 
 The function returns the specified string parameter as its value. That string is assumed
 large enough to hold the number.
 
 Note, this routine is provided as a substitute for sprintf().  It is more compact, faster,
 and doesn't "drag" in a lot of I/O library routines with it.
*/

CM_CHAR *cmltostr(CM_LONG n, CM_SHORT width, CMBoolean hexConversion, CM_CHAR *s)
{
	CMBoolean 	leading0s, neg = false;
	CM_SHORT 		i, len, padding, w;
	CM_CHAR 	 	filler, tmpStr[30], *d, *t, c1, c2, *s0 = s;
	
	/* Convert the number to a string with NO leading zeros.  A zero value is just the		*/
	/* string "0". After we have the string we "format" it.	Note, when we get done with		*/
	/* this conversion, t will point to the first digit of the string.										*/
	
	if (n == 0)															/* handle 0 specially													*/
		strcpy(t = tmpStr, "0");
	else if (hexConversion) {								/* converting to hex...												*/
		for (t = tmpStr, d = (CM_CHAR *)&n, leading0s = true, i = sizeof(long); i; i--, d++) {
			c1 = "0123456789ABCDEF"[(*d >> 4) & 0x0F];
			c2 = "0123456789ABCDEF"[*d & 0x0F];
			if (leading0s) {
				if (c1 != '0') {
					*t++ = c1;
					leading0s = false;
				}
			} else
				*t++ = c1;
			if (leading0s) {
				if (c2 != '0') {
					*t++ = c2;
					leading0s = false;
				}
			} else
				*t++ = c2;
		} /* for */
		*t = '\0';
		t = tmpStr;
	} else {																	/* converting to decimal...									*/
		if (n < 0) {
			neg = true;
			n = -n;
		}
		t = tmpStr + 15;												/* the digits go in from low to high				*/
		*t-- = 0;
		do {
			*t-- = "0123456789"[n % 10];					/* done in a "portable" way!								*/
			n /= 10;
		} while (n);
		if (neg) 
			*t = '-';
		else
			++t;
	}
	
	/* Pad the string according to the width parameter.  If the width is too small, the		*/
	/* entire number is returned overflowing the width.  If width large enough, pad with	*/
	/* 0's or blanks according to whether width is negative or positive respectively.			*/
	
	len = (short)strlen(t);										/* len is converted string width width			*/
	w = (short)((width < 0) ? -width : width);/* w is abs(width)													*/
	
	if (len < w) {														/* converted string fits in width...				*/
		padding = (short)(w - len);							/* ...this is how many pad chars we need		*/
		if (width > 0)													/* ...if width > 0...												*/
			filler = ' ';													/* ...the padding is blanks									*/
		else {																	/* ...if width < 0...												*/
			filler = '0';													/* ...the padding is 0's, but...						*/
			if (neg) {														/* ...if decimal conversion and negative...	*/ 
				--padding;													/* ...reduce amount of padding to put in "-"*/
				*t = '0';														/* ...clobber the "-" already in string			*/
				*s++ = '-';													/* ...1st output char is the sign						*/
			}
		}
		while (padding--) *s++ = filler;				/* ...put in the padding										*/
	}
	
	strcpy(s, t);															/* copy the digits to the output						*/
	return (s0);
}
														 
														  CM_END_CFUNCTIONS
