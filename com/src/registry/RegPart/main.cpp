/* @(#)Z 1.10 com/src/registry/RegPart/main.cpp, odconfig, od96os2, odos29646d 96/11/15 15:49:39 (96/08/19 21:52:19) */

//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odconfig
//
//   CLASSES: none
//
//   ORIGINS: 27
//
//
//   (C) COPYRIGHT International Business Machines Corp. 1995,1996
//   All Rights Reserved
//   Licensed Materials - Property of IBM
//   US Government Users Restricted Rights - Use, duplication or
//   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
//   	
//   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
//   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
//   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
//   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
//   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
//   OR PERFORMANCE OF THIS SOFTWARE.
//
//====END_GENERATED_PROLOG========================================
//

#include <stdio.h>
#include <iostream.h>
#include <string.h>
#include <locale.h>
#ifdef _PLATFORM_AIX_
#include <nl_types.h>
#else
#include <odnltyps.h>
#endif
#include "ISOStr.h"
#include "ODTypesF.h"
#include "RegUtils.h"
#include "ODRegApi.h" 
#include "ODRegUtl.h" 

#define PROGNAME "odRgPrt"
#define SIZE 2048

int main(int argc, char *argv[])
{
	unsigned long program_rc = 0 ;	// what this .exe returns to caller
	unsigned long rc;
        unsigned long size = SIZE;
	char *list;
        char *ret;
	nl_catd regCatHandle;

	setlocale(LC_MESSAGES, NULL);
	regCatHandle = catopen("ODRegUtl.cat", 0);
	if (regCatHandle == CATD_ERR)
		printf(MSGCAT_NOTOPEN);

	if (argc == 1)                    // incorrect number of arguments
	{
		printf(catgets(regCatHandle, REGUTILS_SET, REGUTILS_10, 
				MISSING_ARG), PROGNAME);
		printf(catgets(regCatHandle, REGPART_SET, REGPART_10, 
			REGPART_USAGE));
		exit(-1);
	} 
	else
		printf(catgets(regCatHandle, REGPART_SET, REGPART_20, 
				REGPART_PART_NAME), argv[1]);

	rc = ODRegisterPartHandlerClass((ISOString) argv[1]);
	if ((rc == PR_NO_ERROR) || (rc == errPR_PART_HANDLER_INFO_REPLACED))
	{       
	// query the class to make sure it got registered 
                char list[SIZE]; 
		int rc = ODQueryPartHandlerList(0, list,(ODULong) &size);
		if (rc == RM_NO_ERROR) 
		{
			printf(catgets(regCatHandle, REGUTILS_SET, REGUTILS_20,
				 PART_HANDLER_LIST), list);
			printf(catgets(regCatHandle, REGUTILS_SET, REGUTILS_30,
				 UPDATE_MADE), PROGNAME);
			program_rc = 0 ;	// failure
		}
                else   // Query failed, check dump to see if it registered.
                {
	                ret = err[rc];
			program_rc = 1 ;	// failure
			printf(catgets(regCatHandle, REGPART_SET, REGPART_30, 
					REGPART_ERR), PROGNAME, ret);
	                if ( (rc == errPR_COULD_NOT_LOAD_CLASS) || 
	                     (rc == errPR_PART_HANDLER_NOT_FOUND) || 
	                     (rc == errPR_PART_KIND_NOT_FOUND) )
			{
	                	printf(catgets(regCatHandle, REGUTILS_SET, REGUTILS_40, 
					REG_CHKS), PROGNAME);
			}
#ifdef DEBUG
		        printf("Querying Part Handler Unsuccessful.\n");
		        printf("Part may be registered, run oddumprg to check.\n");
		        printf("If not registered, please try to re-register.\n");
#endif
                }

	}
	else
        {
                ret = err[rc];
		program_rc = 2 ;		// failure
		printf(catgets(regCatHandle, REGPART_SET, REGPART_30, 
				REGPART_ERR), PROGNAME, ret);
                if ( (rc == errPR_COULD_NOT_LOAD_CLASS) || 
                     (rc == errPR_PART_HANDLER_NOT_FOUND) || 
                     (rc == errPR_PART_KIND_NOT_FOUND) )
                   printf(catgets(regCatHandle, REGUTILS_SET, REGUTILS_40, 
				REG_CHKS), PROGNAME);
#ifdef DEBUG
                printf("Register Part Handler Unsuccessful.\n\n");
#endif
	}

	exit (program_rc);

}

