/* @(#)Z 1.7 os2/src/docshell/OS2Main.cpp, odshell, od96os2, odos29646d 96/11/15 15:51:38 (96/10/24 16:56:37) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odshell
//
//   CLASSES: none
//
//   ORIGINS: 82,27
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

//===========================================================================
// Main.cpp
//
//	This file contains the main entry point for the OS2 version of
//	the OpenDoc document shell.
//
//===========================================================================

#include <ODos2.h>

#include <ODDebug.h>
#include <ODTypes.h>
#include <ODUtils.h>
#include <OS2Shell.xh>

#define MAXARGS 8	// Max number of command line args


//===========================================================================
// Main
//
//	The required OS/2 entry point.  It instantiates a OS2Shell
//	and starts it.
//===========================================================================
INT main (int argc, char *argv[])
{
    long		returnValue = FALSE;
    WindowSystemData	windowSystemData;
    Environment 	*ev = somGetGlobalEnvironment();

    try
    {

        // Required initialization prior to any SOM calls ...
        ODInitExceptions();

        // Create a WinShell and start it
        OS2Shell *shell = new OS2Shell;
        if (shell)
        {
	    // Ready, Set, ...
            returnValue = shell->Go(ev, argc, argv);

            // When shell->Go returns we're through
            ODDeleteObject(shell);
        }

        ODTermExceptions();
    }
    catch (ODException _exception)
    {
        WARNMSG(WARN_INDEX(AMSG_510), "Exception %d returned to main.", 
                ErrorCode());
    }

    return returnValue;
}
