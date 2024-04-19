/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odui
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
/* @(#) 1.3 com/src/ui/ODCnrVw.h, odui, od96os2, odos29646d 10/31/96 12:45:40 [11/15/96 15:52:42] */
/*
  File:    odcnrvw.h

  Contains:  Procedural interfaces to the ODViewExtension component

  Written by:  Ajay Apte

  Copyright:  1993-1994 IBM


  To Do:
*/

#ifndef _ODCNRVWH_
#define _ODCNRVWH_

#define INCL_WINHELP
#define INCL_DOS
#define INCL_WINPOINTERS
#define INCL_WINWINDOWMGR
#define INCL_WIN
#define INCL_BASE
#define INCL_GPI
#define INCL_WPCLASS
#define INCL_DEV
#define INCL_DOSMODULEMGR
#define INCL_PM
#define INCL_ODAPI
#include <os2.h>
#define INCL_WINSTDCNR
#include <pmstddlg.h>

// By default 10 columns are shown in the details view.
#define DEFAULT_NUMOFCOLS   10

// The split bar occurs after first 2 columns (icon and title).
#define CX_SPLITBAR 120

// All the title strings,  default icons are loaded from ODRes.dll
#define ODRES_DLL "odres.dll"

class ODPart;
class ODFrame;
typedef struct _ODINFO {
 MINIRECORDCORE              RecordCore;             // MINIRECORDCORE struct
 ULONG        cb;                     // Strcuture size
 ODPart*        part;                   // part object reference
 string       partCategory;           // stored part category
 string            partKind;               // stored part kind
 string            partEditor;             // preferred part editor
 string            partCreateDate;         // initial creation date
 string            partCreateTime;         // initial creation time
 string            partLastModifyDate;     // last modification date
 string            partLastModifyTime;     // last modification time
 string            partLastModifyUser;     // last modifier
 ULONG          partSize;               // size in bytes of part
 struct _ODINFO  *BranchRoot;           // The container that is the parent
 struct _ODINFO  *Leaf;                 // If this is a container, points
 struct _ODINFO  *Next;                 // The next part embedded in the
 ODFrame*      partFrame;          // the Frame for the part. 141192
} ODINFO;
typedef ODINFO *PODINFO;

class ODCnrView;

typedef struct _ODCnrViewList {
   ODCnrView *ContainerView;
   ULONG viewType;
   struct _ODCnrViewList   *Next;
} ODCnrViewList;
typedef ODCnrViewList *PODCneViewList;

//--------------------------Structures-----------------------------
// added for defect 141192 - menu/popup for Container Views

extern "C" {

  typedef struct _ODINFO2
  {
    MINIRECORDCORE      minirecordCore;
    PSZ                 DraftTitle;
    USHORT              DraftNumber;
    PSZ                 CreatorUserName;
    CDATE               date;
    CTIME               time;
    PSZ                 Comments;

  } ODINFO2, *PODINFO2;


}
#endif // _ODCNRVWH_
