/* @(#)Z 1.4 com/src/imaging/Matrix.h, odimaging, od96os2, odos29646d 96/11/15 15:25:04 (96/10/29 09:25:10) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odimaging
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
	File:		Matrix.h

	Contains:	Matrix math [private] for ODTransforms.

	Owned by:	Jens Alfke (conversion to OpenDoc)
	Written by:	Based on QuickDraw GX code by Cary Clark, Michael Fairman,
				Robert Johnson, Keith McGreggor, Oliver Steele, David Van Brink

	Copyright:	© 1987-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):
	
		 <1>	 6/15/94	jpa		first checked in
		 ---------------------------Moved to ODSOM project.
		 <1>	  5/9/94	jpa		first checked in
	To Do:
*/


#ifndef _MATRIX_
#define _MATRIX_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif


extern const ODMatrix kODIdentityMatrix;


ODTransformType MxType( const ODMatrix* );

ODBoolean	MxNormalize( ODMatrix* );
ODBoolean	MxInverse( const ODMatrix*, register ODTransformType,
					   ODMatrix *imatrix);
ODBoolean	MxMul( const ODMatrix*, ODTransformType,
				   ODPoint *vector, ODSLong cnt);
ODBoolean	MxConcat( const ODMatrix *a, ODTransformType aFlags,
					  ODMatrix *b, ODTransformType bFlags );		// b = b * a
ODBoolean	MxMove( ODMatrix*, ODTransformType, ODFixed tx, ODFixed ty );
ODBoolean	MxMoveTo( ODMatrix*, ODTransformType, ODFixed x, ODFixed y);
ODBoolean	MxScale( ODMatrix*, ODFixed sx, ODFixed sy);
ODBoolean	MxSkew( ODMatrix*, ODFixed sx, ODFixed sy);
ODBoolean	MxRotate( ODMatrix*, ODFixed angle );


#endif /*_MATRIX_*/
