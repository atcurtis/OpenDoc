/* @(#)Z 1.3 com/src/registry/PartInfo.cpp, odconfig, od96os2, odos29646d 96/11/15 15:49:09 (96/10/29 09:29:19) */
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

/*
 *  This file was generated by the SOM Compiler.
 *  Generated using:
 *     SOM incremental update: somc/smmain.c
 */


/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using:
 *      SOM Emitter emitxtm: somc/smmain.c
 */

#define ODPartHandlerInfo_Class_Source

#include <ODTypes.h>

#include <PartInfo.xih>

#ifdef _PLATFORM_OS2_
#include <ODPagtun.h>
#endif

/*
 * SOM_Scope void  SOMLINK ODPartHandlerInfoInitODPartHandlerInfo(ODPartHandlerInfo *somSelf,
 *                                                                 Environment *ev,
 *                                                                string partClassName,
 *                                                                string DLLName,
 *                                                                OperatingSystem os)
 */

/*
 * The prototype for ODPartHandlerInfoInitODPartHandlerInfo was replaced by the following prototype:
 */
SOM_Scope long  SOMLINK ODPartHandlerInfoInitODPartHandlerInfo(ODPartHandlerInfo *somSelf,
                                                                Environment *ev,
                                                               string partClassName,
                                                               string DLLName,
                                                               OperatingSystem os)
{
    /* ODPartHandlerInfoData *somThis = ODPartHandlerInfoGetData(somSelf); */
    ODPartHandlerInfoMethodDebug("ODPartHandlerInfo","ODPartHandlerInfoInitODPartHandlerInfo");

    /* Return statement to be customized: */
    { long retVal;  return (retVal); }
}

SOM_Scope string  SOMLINK ODPartHandlerInfoGetPartHandlerClassName(ODPartHandlerInfo *somSelf,
                                                                    Environment *ev)
{
    /* ODPartHandlerInfoData *somThis = ODPartHandlerInfoGetData(somSelf); */
    ODPartHandlerInfoMethodDebug("ODPartHandlerInfo","ODPartHandlerInfoGetPartHandlerClassName");

    /* Return statement to be customized: */
    { string retVal;  return (retVal); }
}

SOM_Scope ISOString  SOMLINK ODPartHandlerInfoGetPartHandlerName(ODPartHandlerInfo *somSelf,
                                                                  Environment *ev)
{
    /* ODPartHandlerInfoData *somThis = ODPartHandlerInfoGetData(somSelf); */
    ODPartHandlerInfoMethodDebug("ODPartHandlerInfo","ODPartHandlerInfoGetPartHandlerName");

    /* Return statement to be customized: */
    { ISOString retVal;  return (retVal); }
}

SOM_Scope string  SOMLINK ODPartHandlerInfoGetPartHandlerDisplayName(ODPartHandlerInfo *somSelf,
                                                                      Environment *ev)
{
    /* ODPartHandlerInfoData *somThis = ODPartHandlerInfoGetData(somSelf); */
    ODPartHandlerInfoMethodDebug("ODPartHandlerInfo","ODPartHandlerInfoGetPartHandlerDisplayName");

    /* Return statement to be customized: */
    { string retVal;  return (retVal); }
}

SOM_Scope _IDL_SEQUENCE_PartKindInfo  SOMLINK ODPartHandlerInfoGetPartKindInfo(ODPartHandlerInfo *somSelf,
                                                                                Environment *ev,
                                                                               string category)
{
    /* ODPartHandlerInfoData *somThis = ODPartHandlerInfoGetData(somSelf); */
    ODPartHandlerInfoMethodDebug("ODPartHandlerInfo","ODPartHandlerInfoGetPartKindInfo");

    /* Return statement to be customized: */
    { _IDL_SEQUENCE_PartKindInfo retVal;  return (retVal); }
}

SOM_Scope OperatingSystem  SOMLINK ODPartHandlerInfoGetOperatingSystem(ODPartHandlerInfo *somSelf,
                                                                        Environment *ev)
{
    /* ODPartHandlerInfoData *somThis = ODPartHandlerInfoGetData(somSelf); */
    ODPartHandlerInfoMethodDebug("ODPartHandlerInfo","ODPartHandlerInfoGetOperatingSystem");

    /* Return statement to be customized: */
    { OperatingSystem retVal;  return (retVal); }
}

SOM_Scope void  SOMLINK ODPartHandlerInfoSetOperatingSystem(ODPartHandlerInfo *somSelf,
                                                             Environment *ev,
                                                            OperatingSystem os)
{
    /* ODPartHandlerInfoData *somThis = ODPartHandlerInfoGetData(somSelf); */
    ODPartHandlerInfoMethodDebug("ODPartHandlerInfo","ODPartHandlerInfoSetOperatingSystem");

}

SOM_Scope string  SOMLINK ODPartHandlerInfoGetOLE2ClassId(ODPartHandlerInfo *somSelf,
                                                           Environment *ev)
{
    /* ODPartHandlerInfoData *somThis = ODPartHandlerInfoGetData(somSelf); */
    ODPartHandlerInfoMethodDebug("ODPartHandlerInfo","ODPartHandlerInfoGetOLE2ClassId");

    /* Return statement to be customized: */
    { string retVal;  return (retVal); }
}

SOM_Scope string  SOMLINK ODPartHandlerInfoGetWindowsIconFileName(ODPartHandlerInfo *somSelf,
                                                                   Environment *ev)
{
    /* ODPartHandlerInfoData *somThis = ODPartHandlerInfoGetData(somSelf); */
    ODPartHandlerInfoMethodDebug("ODPartHandlerInfo","ODPartHandlerInfoGetWindowsIconFileName");

    /* Return statement to be customized: */
    { string retVal;  return (retVal); }
}

SOM_Scope string  SOMLINK ODPartHandlerInfoGetDLLName(ODPartHandlerInfo *somSelf,
                                                       Environment *ev)
{
    /* ODPartHandlerInfoData *somThis = ODPartHandlerInfoGetData(somSelf); */
    ODPartHandlerInfoMethodDebug("ODPartHandlerInfo","ODPartHandlerInfoGetDLLName");

    /* Return statement to be customized: */
    { string retVal;  return (retVal); }
}
