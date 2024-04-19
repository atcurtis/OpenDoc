/* @(#)Z 1.9 com/src/registry/DumpReg/RegUtils.h, odconfig, od96os2, odos29646d 96/11/15 15:49:38 (96/10/29 09:28:56) */
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


// default common messages for registry utilities
// ----------------------------------------------
#define MSGCAT_NOTOPEN "Could not open message catalog ODRegUtl.cat.\n"
#define MISSING_ARG "%s: Missing argument.\n\n"
#define PART_HANDLER_LIST "The Part Handler list is %s\n\n"
#define UPDATE_MADE "%s: Update made to the Part Registry will be\n\
   in effect for new OpenDoc sessions only. Existing\n\
   OpenDoc session will not contain the update made to\n\
   the Part Registry.\n"
#define REG_CHKS "%s: Please check for the following problems:\n\
   1) You must register your part with SOM by running sc on your idl file.\n\
   2) Your SOMIR environment variable must contain the Interface Repository you registered with.\n\
      (i.e. opendoc.ir) \n\
   3) Your part\'s dll directory must be in the search path defined in the appropriate environment variable.\n\
      (PATH or LIBPATH) \n\
   4) Names are case sensitive! and must be the class name for register and \n\
      deregister utilities. \n\n\
Please check these items and run again. \n"
 

char *err[] = {  "SUCCESSFUL",
                 "INVALID_CLASSNAME",
                 "COULD_NOT_LOAD_CLASS",
                 "NOT_A_PART_HANDLER_CLASS",
                 "PART_HANDLER_INFO_REPLACED_SUCCESSFULLY",
                 "PART_HANDLER_WAS_NOT_FOUND", 
                 "PART_KIND_WAS_NOT_FOUND",
                 "YOUR_BUFFER_TOO_SMALL"
             };


// default odRegPart messages
// --------------------------
#define REGPART_USAGE "Usage: odRegPrt <partname>\n\
   where <partname> is the (Case Sensitive) class name of the new part handler\n"
#define REGPART_PART_NAME "Part name = %s\n"
#define REGPART_ERR "%s: %s returned when attempted to install\n\
   the part handler.\n"

// default odDeRegPt messages
// --------------------------
#define DEREGPART_USAGE "Usage: odDeRgPt <partname>\n\
   where <partname> is the (Case Sensitive) class name of the registered\n\
                    part handler\n"
#define DEREGPART_ERR "%s: %s returned when attempted to remove\n\
   a registered part handler.\n"

// default odDumpRg messages
// -------------------------

#define DUMPREG_USAGE "\n\
Usage:\n\ 
       odDumpRg                     - Returns lists of info from registry file\n\
 -or-  odDumpRg 1 \"yourPartKind\"    - Given a partKind, returns a partHandler\n\
 -or-  odDumpRg 2 \"yourPartHandler\" - Given a partHandler, returns partHandler\n\
                                        info\n\
 -or-  odDumpRg 3 \"yourPartHandler\" \"yourPartKind\" \n\
                  - Given a partHandler, returns partHandler info\n"


#define DUMPREG_PART_KIND_LIST "The Part Kind list is %s\n\n"
#define DUMPREG_PART_LIST "The Part Handler list for %s is %s\n"
#define DUMPREG_PERF_PART_LIST "The Preferred Part Handler for %s is %s\n\n"
#define DUMPREG_PART_HANDLE_INFO "The Part Handler Info for %s is: \n"
#define DUMPREG_PART_HANDLE_NAME "PartHandlerName %s\n"
#define DUMPREG_PART_CLASS_NAME "PartHandlerClassName %s\n"
#define DUMPREG_PART_HANDLE_DISPLAY_NAME "PartHandlerDisplayName %s\n"
#define DUMPREG_OLE_CLASS_ID "ole2ClassID %s\n"
#define DUMPREG_PART_KIND_INFO "The Part Kind Info for %s is: \n"
#define DUMPREG_PART_KIND_NAME "PartKindName %s\n"
#define DUMPREG_PART_KIND_DISPLAY_NAME "PartKindDisplayName %s\n"
#define DUMPREG_PART_KIND_TYPE_NAME "PartKindTypeName %s\n"
#define DUMPREG_PART_KIND_CATEGORY "PartKindCategories %s\n\n"
#define DUMPREG_PART_KIND_DisplayName "PartKindDisplayName %s\n\n"
#define DUMPREG_PART_Handler_DisplayName "PartHandlerDisplayName %s\n\n"
#define DUMPREG_ERR1 "%s: %s returned when attempted to retrieve\n\
   data from the Part Registry.\n\n"
#define DUMPREG_ERR2 "%s: Not able to get the Part Handler list for\n\
   %s\n\n"




