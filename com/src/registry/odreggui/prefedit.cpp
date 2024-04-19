/* @(#)Z 1.5 os2/src/registry/odreggui/prefedit.cpp, odconfig, od96os2, odos29646d 96/11/15 15:52:48 (96/09/12 14:46:53) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odconfig
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

// *********************************************************************                           `
//
//  File:      prefedit.cpp
//  Contains:  Registry GUI source
//
// ************************************************************************

//#define INCL_WIN
//#include <os2.h>


#include <odos2.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <odnltyps.h>
#include "prefedit.h"
#include "odregutl.h"
#include <iostream.h>
#include <ISOStr.h>
#include <ODRegApi.h>
#include <ODTypes.h>

MRESULT EXPENTRY DialogFunc(HWND,ULONG,MPARAM,MPARAM);

char **parse_partslist(char *list,unsigned long size,int& index);
unsigned long set_editor_pref(char *editor);

HPOINTER hIcon;

HMODULE hmod;

nl_catd regCatHandle;

int main(VOID)

{
   HAB hab;
   HMQ hmq;
   HWND hwnd;
   QMSG qmsg;
   SWP swp;
   CHAR szClientClass[] = "Client Window";

   hab = WinInitialize(0);
   hmq = WinCreateMsgQueue(hab, 0);


   // Load in the resource
   char LoadError[256] = "";
   int rc = 0;
   rc = DosLoadModule(LoadError, sizeof(LoadError), "ODPREFED", &hmod);
   if (rc != 0 )
      printf(catgets(regCatHandle, REGGUI_SET, REGGUI_70, RC_NOT_OPEN));

    setlocale(LC_MESSAGES, NULL);
    regCatHandle = catopen("ODRegUtl.cat", 0);
    if (regCatHandle == CATD_ERR)
       printf("Cannot open message catalog ODRegUtl.cat.\n");

   WinRegisterClass(hab,                    // Anchor block handle
      szClientClass,                       // Name of class being registered
      (PFNWP)DialogFunc,                    // dialog procedure for class
      0,                                    // Class style
      0);                                   // Extra bytes to reserve

   hwnd = WinLoadDlg(HWND_DESKTOP,          // parent
      HWND_DESKTOP,                         // owner
      DialogFunc,                           // dialog window proc
      hmod,                                 // module handle for resource
      ID_DIALOG,                            // dialog template
      NULL);                                // no app data

   // load ICON
   hIcon = WinLoadPointer( HWND_DESKTOP, 0, IDD_ICON );

   //find where system would place dialog
   WinQueryTaskSizePos(hab, 0, &swp);

   // place dialog box at position given by shell
   WinSetWindowPos(hwnd, NULLHANDLE, swp.x, swp.y, 0, 0, SWP_SHOW|SWP_MOVE);

   if (hwnd)
      while (WinGetMsg(hab, &qmsg, NULLHANDLE, 0, 0))
         WinDispatchMsg(hab, &qmsg);

   WinDestroyWindow(hwnd);                  // cleanup
   WinDestroyMsgQueue(hmq);
   WinTerminate(hab);
   return 0;
}

//--------------------------------------------------------------------
// Dialog Box Function for selecting preferred editor
//--------------------------------------------------------------------

MRESULT EXPENTRY DialogFunc(HWND hdwnd,ULONG message,MPARAM mp1,MPARAM mp2)
{

   enum
      { BUFFER_SIZE = 1024,
        MAX_STRING = 256 } ;

   int i, i2 = 0;
   int &index = i2;
   char str[254];
   static char **kindlist = NULL;
   static char **partlist = NULL;
   char buf_list[BUFFER_SIZE];
   unsigned long rc;
   unsigned long size;
   char displayname[BUFFER_SIZE];


   switch (message)
   {
      //------------------------------------------------------
      //  Initialize the Part Kind listbox, display Kinds list
      //------------------------------------------------------
   case WM_INITDLG:

         // Check for existance of any parts
         size = BUFFER_SIZE;
         rc = ODQueryPartKindList(0, buf_list, (ODULong)&size);

         if (rc != RM_NO_ERROR)
            {
            sprintf(str,
                    catgets(regCatHandle, REGGUI_SET, REGGUI_40, KIND_FAIL),
                    rc);
            WinMessageBox(HWND_DESKTOP, hdwnd, str,
                       catgets(regCatHandle, REGGUI_SET, REGGUI_10, WARN_TITLE),
                       0, MB_OK);
            WinPostMsg(hdwnd, WM_QUIT, 0, 0);
            return MRESULT(FALSE);
            }

         // Get a list of registered Part Kinds
         kindlist = parse_partslist(buf_list, size, index);

         for (i = 0; i <= index; i++)
            {
            size = BUFFER_SIZE;
            rc = ODQueryKindDisplayName(kindlist[i],  displayname, (ODULong) &size);

            WinSendDlgItemMsg(hdwnd, IDD_KINDBOX, LM_INSERTITEM, MPFROMSHORT(LIT_END),
               MPFROMP(displayname));
            }
         WinSendDlgItemMsg(hdwnd, IDD_KINDBOX, LM_SELECTITEM, MPFROMSHORT(0),
            MPFROMSHORT(TRUE));

         // disable OK key until part handlers are listed
         WinEnableControl(hdwnd, DID_OK, FALSE);

         break;

      case  WM_CONTROL:

         switch (SHORT1FROMMP(mp1))
         {

            //------------------------------------------------
            //  Part Kind Listbox
            //------------------------------------------------
            case  IDD_KINDBOX: // user double clicks Kinds for Part Handler List

               if (SHORT2FROMMP(mp1) == LN_ENTER)
               {
                  i = (int)WinSendDlgItemMsg(hdwnd, IDD_KINDBOX, LM_QUERYSELECTION, 0, 0);
                  WinSendDlgItemMsg(hdwnd, IDD_KINDBOX, LM_QUERYITEMTEXT, MPFROM2SHORT((i),(MAX_STRING)),
                                    MPFROMP(str));

                  // Get a list of registered Part Handlers bases on kindlist with item_index
                  size = BUFFER_SIZE;
                  rc = ODQueryPartHandlerList(kindlist[i], buf_list, (ODULong)&size);
                  if (rc != RM_NO_ERROR)
                     {
                     sprintf(str,
                             catgets(regCatHandle, REGGUI_SET, REGGUI_30, PART_FAIL),
                             rc);
                     WinMessageBox(HWND_DESKTOP, hdwnd, str,
                       catgets(regCatHandle, REGGUI_SET, REGGUI_10, WARN_TITLE),
                        0, MB_OK);
                     return 0;
                     }

                  // populate the part editor listbox
                  WinSendDlgItemMsg(hdwnd, IDD_PARTBOX, LM_DELETEALL, 0, 0);
                  free(partlist);

                  partlist = parse_partslist(buf_list, size, index);


                  for (i = 0; i <= index; i++)
                     {
                     size = BUFFER_SIZE;
                     rc = ODQueryHandlerDisplayName(partlist[i], displayname, (ODULong) &size);

                     WinSendDlgItemMsg(hdwnd, IDD_PARTBOX, LM_INSERTITEM, MPFROMSHORT(LIT_END),
                        MPFROMP(displayname));
                     }

                  // Set the cursor position to the 1st entry.
                  WinSendDlgItemMsg(hdwnd, IDD_PARTBOX, LM_SELECTITEM, MPFROMSHORT(0),
                                    MPFROMSHORT(TRUE));
                  // enable OK key, part handlers are listed
                  WinEnableControl(hdwnd, DID_OK, TRUE);

               }  // if (SHORT2FROMMP(mp1) == LN_ENTER)
               break;

            default:
               return  WinDefDlgProc(hdwnd, message, mp1, mp2);

         } // switch (SHORT1FROMMP(mp1))
         break;

      case  WM_COMMAND:

         switch (SHORT1FROMMP(mp1))
         {

            //------------------------------------------------
            //  OK PushButton - Set Editor Preference & exit
            //------------------------------------------------
         case DID_OK:

               i = (int)WinSendDlgItemMsg(hdwnd, IDD_PARTBOX, LM_QUERYSELECTION, 0, 0);
               WinSendDlgItemMsg(hdwnd, IDD_PARTBOX, LM_QUERYITEMTEXT, MPFROM2SHORT((i),(MAX_STRING)),
                                 MPFROMP(str));

               rc = set_editor_pref(partlist[i]);
               if (rc != RM_NO_ERROR)
                  {
                  sprintf(str,
                          catgets(regCatHandle, REGGUI_SET, REGGUI_50, REG_FAIL),
                          rc);
                  WinMessageBox(HWND_DESKTOP, hdwnd, str,
                     catgets(regCatHandle, REGGUI_SET, REGGUI_10, WARN_TITLE),
                                0, MB_OK);
                  return 0;
                  }
               free(kindlist);
               free(partlist);
               WinPostMsg(hdwnd, WM_QUIT, 0, 0);
               return MRESULT(FALSE);
            //------------------------------------------------
            //  Cancel PushButton - Exit dialog box w/o change
            //------------------------------------------------
            case  DID_CANCEL:
               free(kindlist);
               free(partlist);
               WinPostMsg(hdwnd, WM_QUIT, 0, 0);
               return MRESULT(FALSE);
            //------------------------------------------------
            //  Help PushButton - Temporary Message Box Help
            //------------------------------------------------
            case  IDD_HELP:
               WinMessageBox(HWND_DESKTOP, hdwnd,
                    catgets(regCatHandle, REGGUI_SET, REGGUI_60, HELP_MSG),
                    "", 0, MB_OK);
               return MRESULT(FALSE);

         } // switch (SHORT1FROMMP(mp1))

      default:
         return  WinDefDlgProc(hdwnd, message, mp1, mp2);

   } // switch (message)
  return MRESULT(FALSE);
}

//----------------------------------------------------------------------
//  parse_partslist
//----------------------------------------------------------------------

char **parse_partslist(char *list,unsigned long size,int& index)
{
   int i = 0;
   char *common,*wd,*end;

   wd = list;
   end = list+size;
   char **namelist = (char **)malloc((size/2)*sizeof(char *));
     // Build an array using the part Handler list returned from
     // ODQueryPartHandlerList with each array element containing
     // a part kind

   while (wd < end)
      {

      if (common = strchr(wd, ','))
         {
         namelist[i] = (char *)malloc(common-wd+1);
         *(namelist[i]+(common-wd)) = '\00';
         memcpy(namelist[i++], wd, common-wd);
         wd = common+1;
         }

      else
         {
         namelist[i] = (char *)malloc(end-wd+1);
         *(namelist[i]+(end-wd)) = '\00';
         memcpy(namelist[i], wd, end-wd);
         wd = end;
         }
      }
   index = i;

   return (namelist);
}

//----------------------------------------------------------------------
//  SetEditorPref
//----------------------------------------------------------------------

unsigned long set_editor_pref(char *editor)
{

   int rc = 0;

   //--------------------------------------------------
   // uninstall selected editor
   //--------------------------------------------------
   rc = ODDeregisterPartHandler(editor);

   if (rc == RM_NO_ERROR)
      {

     //--------------------------------------------------
     // reinstall this editor (makes it the "preferred editor")
     //--------------------------------------------------
      rc = ODRegisterPartHandlerClass((ISOString)editor);
      }

   return (rc);
}
