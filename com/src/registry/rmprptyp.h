/* @(#)Z 1.4 com/src/registry/rmprptyp.h, odconfig, od96os2, odos29646d 96/11/15 15:49:28 (96/10/29 09:30:17) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odconfig
 *
 *   CLASSES: none
 *
 *   ORIGINS: 27
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

#ifndef _RMPRPTYP_
#define _RMPRPTYP_


enum
{
    CM_READONLY,
    CM_READWRITE
};

// ***************************************************************************
// * Bento value type constants
// ***************************************************************************
 
const char* const kRMContainerType       = "RM:TYPE:RMContainer";
const char* const kRMISOStringType       = "RM:TYPE:ISOString";
const char* const kRMstringType          = "RM:TYPE:string";
const char* const kRMsequenceType        = "RM:TYPE:sequence";
const char* const kRMOperatingSystemType = "RM:TYPE:OperatingSystem";
const char* const kRMlongType            = "RM:TYPE:long";    
const char* const kRMPartKindInfoType    = "RM:TYPE:PartKindInfo";

// ***************************************************************************
// * Bento property constants
// ***************************************************************************
 
const char* const kRMpartHandlerNameProp        = "RM:PROP:partHandlerName";
const char* const kRMpartHandlerDisplayNameProp = "RM:PROP:partHandlerDisplayName";
const char* const kRMpartHandlerClassNameProp   = "RM:PROP:partHandlerClassName";
const char* const kRMDLLNameProp                = "RM:PROP:DLLName";
const char* const kRMpartKindInfoProp           = "RM:PROP:partKindInfo";
const char* const kRMole2ClassIdProp            = "RM:PROP:ole2ClassId";
const char* const kRMwindowsIconFilenameProp    = "RM:PROP:windowsIconFilename";
const char* const kRMosProp                     = "RM:PROP:os";
                 
const char* const kRMpartKindNameProp            = "RM:PROP:partKindName";        
const char* const kRMpartKindDisplayNameProp     = "RM:PROP:partKindDisplayName"; 
const char* const kRMfilenameFiltersProp         = "RM:PROP:filenameFilters";     
const char* const kRMfilenameTypesProp           = "RM:PROP:filenameTypes";     
const char* const kRMcategoriesProp              = "RM:PROP:categories";          
const char* const kRMcategoryDisplayNameProp     = "RM:PROP:categoryDisplayName"; 
const char* const kRMobjectIDProp                = "RM:PROP:objectID";          

#endif
