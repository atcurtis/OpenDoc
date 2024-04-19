/* @(#)Z 1.33 com/src/docshell/DraftWn.cpp, odshell, od96os2, odos29646d 96/11/15 15:25:02 (96/10/29 09:23:58) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odshell
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

//
//----------------------------------------------------------------------------
// DraftWn.cpp - This file contains the DraftWindow class implementation.
//               This class invokes and manages the draft history window.
//----------------------------------------------------------------------------
#include "DraftWn.h"
#include "DocShell.h"
#include "ShellUtl.h"

//////////////////////////////////////////////////////////////////////////////
////                           MEMBER FUNCTIONS                           ////
//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------
DraftWindow::DraftWindow(DocumentManager* docmgr, 
                         WindowSystemData* wsdata,
                         ODPlatformWindow hwnd)

{
  fDraftInfoList         = kODNULL,
  fSelectedDraft         = kODNULL;
  fActiveDocumentManager = docmgr;

#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_)
  fWindow                = hwnd;
  fInstance              = wsdata->resInstance;
#elif defined(_PLATFORM_UNIX_)
  fInstance              = wsdata->appShell;
#endif

}

//----------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------
DraftWindow::~DraftWindow() 
{
  Environment *ev = somGetGlobalEnvironment();
  try
  {
    // Free the list of history records.
    fActiveDocumentManager->DeleteDraftList(ev);
  }
  catch (ODException _exception)
  {
     LOG("Exception occured in DraftWindow::~DraftWindow");
     throw;
  }
}

//----------------------------------------------------------------------------
// DraftHistory - invokes the drafthistory dialog.
//----------------------------------------------------------------------------
void DraftWindow::DraftHistory()
{
#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_)
  Environment *ev = somGetGlobalEnvironment();

  try
  {
     // Before creating the dialog, disable mouse and keyboard inputs
     // to all the OD windows except the window that is going to own
     // the dialog. After dismissing the dialog, enable back all the
     // OD windows except the owner window for the dialog.
     EnableApplModal(ev, fWindow, FALSE);
#endif

#if defined (_PLATFORM_WIN32_)
     DialogBoxParam(fInstance, MAKEINTRESOURCE(IDD_DRAFTHISTORYDLG), fWindow, 
                 (FARPROC) DraftDialogFunc, (LPARAM) this);
#elif defined (_PLATFORM_OS2_)
     WinDlgBox(HWND_DESKTOP, fWindow, (PFNWP) DraftDialogFunc, 
                   fInstance, IDD_DRAFTHISTORYDLG, (PVOID) this);
#elif defined (_PLATFORM_UNIX_)
     DraftHistoryDlg(fInstance, this );
#endif
    
#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_)
     EnableApplModal(ev, fWindow, TRUE);
  }
  catch (ODException _exception)
  {
     LOG("Exception occured in DraftWindow::DraftHistory");
     RestoreApplModal(fWindow);
     throw;
  }
  
#endif
}
  
//----------------------------------------------------------------------------
//  BuildEntry - builds the text string for list dialog based on 
//               information in the draft info record.
//----------------------------------------------------------------------------
char* DraftWindow::BuildEntry(DraftInfoRec* draftptr)
{
  const wcsTotalLen = (kODNameLength * kODWcsLength) +
                      (kODNumLength  * kODWcsLength) +
                      (kODDateLength * kODWcsLength) +
                      (kODCmntLength * kODWcsLength) +
                      (3             * kODWcsLength) + // tabs
                      (1             * kODWcsLength);  // Null

  const TotalLen = kODNameLength + kODNumLength +
                   kODDateLength + kODCmntLength +
                   3 + 1;

  wchar_t *wcsContStr               = L"...";
  wchar_t *wcsTabStr                = L"\t";
  wchar_t wcsName[kODMaxLength]     = L"";
  wchar_t wcsNum[kODNumLength]      = L"";
  wchar_t wcsDate[kODDateLength]    = L"";
  wchar_t wcsComments[kODMaxLength] = L"";
  wchar_t wcsDraftEntry[wcsTotalLen]= L"";
  char    mbsName[kODMaxLength]     = "";
  char    mbsNum[kODNumLength]      = ""; 
  char    mbsDate[kODDateLength]    = "";
  char    mbsComments[kODMaxLength] = "";
  char    *mbsDraftEntry;
  ODIText *tmpName, *tmpComment;

  // Get the name of last user to modify draft
  if (tmpName = draftptr->GetModUser())
  {
    GetITextString(tmpName, mbsName);

    // Stuff with blanks in case user does't specify name.
    if (mbsName[0] == '\0')
    {
      memset((void*) mbsName, (int) ' ', kODNameLength);
      mbsName[kODNameLength] = '\0';
    }

    // convert to widechar for manipulation.
    mbstowcs(wcsName, mbsName, strlen(mbsName) * kODWcsLength);

    // If name is longer than allowed in draft entry, need to add "..." to end.
    if (wcslen(wcsName) > kODNameLength) 
    {
      // Force end of string temporarily to set it up for wcscat below.
      wcsName[kODNameLength-3] = L'\0';

      // concatenate continuation string onto input field.
      wcscat(wcsName, wcsContStr);
    }
  }

  // Get the number of the draft and convert it into a wchar_t. 
  sprintf(mbsNum, "%d", draftptr->GetDraftNum());
  mbstowcs(wcsNum, mbsNum, strlen(mbsNum) * kODWcsLength);

  // Get the date and time the draft was last modified.
  ODULong time = draftptr->GetSavedDate();
  if (time)
  {
    char* dateFormat = nl_langinfo(D_T_FMT); 
    struct tm *timeptr = localtime((const time_t*) &time);

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
    wchar_t wcsDateFormat [kODDateLength];
    mbstowcs (wcsDateFormat, dateFormat, kODDateLength * kODWcsLength);
    wcsftime(wcsDate, kODDateLength*kODWcsLength, wcsDateFormat, timeptr);
#elif defined(_PLATFORM_UNIX_)
    wcsftime(wcsDate, kODDateLength*kODWcsLength, dateFormat, timeptr);
#endif
  }

  // Get the comments
  if (tmpComment = draftptr->GetComment())
  {
    GetITextString(tmpComment, mbsComments);

    // Stuff with blanks in case user does't specify comments.
    if (mbsComments[0] == '\0')
    {
      memset((void*) mbsComments, (int) ' ', kODCmntLength);
      mbsComments[kODCmntLength] = '\0';
    }

    // convert to widechar for manipulation.
    mbstowcs(wcsComments, mbsComments, strlen(mbsComments) * kODWcsLength);

    // If comments are longer than allowed in draft entry, need to add "..." 
    if (wcslen(wcsComments) > kODCmntLength) 
    {
      // Force end of string temporarily to set it up for wcscat below.
      wcsComments[kODCmntLength-3] = L'\0';

      // concatenate continuation string onto input field.
      wcscat(wcsComments, wcsContStr);
    }
  }

  if (wcsDraftEntry)
  {
    wcscpy(wcsDraftEntry, wcsName);
    wcscat(wcsDraftEntry, wcsTabStr);
    wcscat(wcsDraftEntry, wcsNum);
    wcscat(wcsDraftEntry, wcsTabStr);
    wcscat(wcsDraftEntry, wcsDate);
    wcscat(wcsDraftEntry, wcsTabStr);
    wcscat(wcsDraftEntry, wcsComments);

    // convert back to mb strings and create entry.
    mbsDraftEntry = (char*) malloc (TotalLen);
    wcstombs(mbsDraftEntry,wcsDraftEntry,wcsTotalLen);
  }
  else
  {
    WARNMSG(WARN_INDEX(AMSG_600),"Out of memory");
    return NULL;
  }

  return mbsDraftEntry;
}

//----------------------------------------------------------------------------
//  FindDraftInList - Finds a particular draft info record based on a 
//                    position passed in.
//----------------------------------------------------------------------------
DraftInfoRec* DraftWindow::FindDraftInList (int ItemSelected)
{
  DraftInfoRec* draftptr;

  // Find selected draft in fDraftInfoList.  

#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_UNIX_)
  // NT and AIX: the ItemSelected paramter is the position in the list
  // of the selected draft.  Assumption is that list order reflects order 
  // of draftinfo records in dialog list.  Therefore position is equivalent.
  draftptr = fDraftInfoList;
  for (int position = 0;
       (draftptr != kODNULL) && (position < ItemSelected);
       position++, draftptr = draftptr->GetNext());
#elif defined(_PLATFORM_OS2_)
  // On Os2 the ItemSelected paramter is the actual draft number, not
  // the position in the list of the draft entry.  Therefore we search
  // the list for the DraftInfoRec that matches the draft number.
  for (draftptr=fDraftInfoList; 
       (draftptr != kODNULL) && (draftptr->GetDraftNum() != ItemSelected); 
       draftptr = draftptr->GetNext());
#endif

  return draftptr;
}

//----------------------------------------------------------------------------
//  OpenDraft - Opens the selected draft.
//----------------------------------------------------------------------------
ODBoolean DraftWindow::OpenDraft(int ItemSelected)
{
  // Create a local SOM environment to pass into SOM class calls used
  // in this method.
  Environment* ev = somGetGlobalEnvironment();
  ODBoolean retCode = kODFalse;
  
  try
  {

    fSelectedDraft = kODNULL;

    // Find draft in list
    DraftInfoRec* draftrec = this->FindDraftInList(ItemSelected);

    // save handle to locked draft.
    if (draftrec) fSelectedDraft = draftrec->GetDraft();

    // Acquire the draft to increment the ref count.
    if (fSelectedDraft)
    {
      fSelectedDraft->Acquire(ev);

      // Open it.
      retCode = fActiveDocumentManager->OpenDraft(ev,fSelectedDraft, kODTrue);
    }
  
  }
  catch (ODException _exception)
  {
    LOG("Exception occured in DraftWn::OpenDraft\n");
    if (fSelectedDraft)
       SaveAndRestoreEv1(fSelectedDraft);
    throw;
  }

  
  return retCode;

}
//----------------------------------------------------------------------------
//  RemoveDraft - Deletes the selected draft and removes it from the 
//                list of draft info records.
//----------------------------------------------------------------------------
ODBoolean DraftWindow::RemoveDraft(int ItemSelected)
{
  // Create local SOM environment to pass into SOM Class
  // calls in this method.
  Environment* ev = somGetGlobalEnvironment();

  ODBoolean retCode = kODFalse;
  
  try
  {

    // Find draft in list
    DraftInfoRec* draftrec = this->FindDraftInList(ItemSelected);

    // save handle to locked draft.
    fSelectedDraft = kODNULL;
    if (draftrec) fSelectedDraft = draftrec->GetDraft();

    // Acquire the draft to increment the ref count.
    if (fSelectedDraft) 
    {
      // Note: Delete Draft takes care of releasing the draft.
      fSelectedDraft->Acquire(ev);
  
      // Delete it.
      retCode = fActiveDocumentManager->DeleteDraft(ev,draftrec);
    }

  }
  catch (ODException _exception)
  {
    LOG("Exception occured in DraftWn::RemoveDraft\n");
    throw;
  }

  return retCode;
}

//----------------------------------------------------------------------------
//  ActiveDocumentManager accessors
//----------------------------------------------------------------------------
void DraftWindow::SetActiveDocumentManager(DocumentManager* docMgr)
{
    fActiveDocumentManager = docMgr;
}

DocumentManager* DraftWindow::GetActiveDocumentManager()
{
    return fActiveDocumentManager;
}

//----------------------------------------------------------------------------
//  DraftInfoList accessors
//----------------------------------------------------------------------------
void DraftWindow::SetDraftInfoList(DraftInfoRec* draftInfoRec)
{
    fDraftInfoList = draftInfoRec;
}

DraftInfoRec* DraftWindow::GetDraftInfoList()
{
    return fDraftInfoList;
}

//----------------------------------------------------------------------------
//  Window accessors
//----------------------------------------------------------------------------
void DraftWindow::SetWindow(ODPlatformWindow window)
{
    fWindow = window;
}

ODPlatformWindow DraftWindow::GetWindow()
{
    return fWindow;
}

