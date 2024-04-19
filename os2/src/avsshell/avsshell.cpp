/* @(#)Z 1.9 os2/src/avsshell/avsshell.cpp, oddataxfer, od96os2, odos29647a 96/11/18 05:19:07 (96/11/15 17:05:53) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: oddataxfer
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

//
// Copyright (C) Apple Computer, Inc., 1994
//
//******************************************************************

#ifdef _PLATFORM_OS2_
   #include <string.h>
   #include <time.h>
   #define INCL_WIN
   #define INCL_GPI
   #define INCL_BASE
   #define INCL_DOSSEMAPHORES
   #define INCL_DOSERRORS
   #define INCL_WINSTDFILE
// comment out for now. ssl
//   #define INCL_OSAAPI
//   #define INCL_OSA
#endif

#include <ISOStr.h>
#include <IODDefs.xh>
#include <ILnkSrvc.xh>
// not found. ssl
//#include <ShSessn.xh>
#include <AvsSessn.xh>
#include <IAvlSvr.xh>
#include <os2.h>
#include <io.h>

// 27480
#include <StdTypIO.h>
#include <StdProps.xh>
#include <StdTypes.xh>
#include <TempObj.h>

// Just a declaration for ShellMain in os2_feb13\opendoc\src\opendoc\docshell\shlmain.h
// #ifndef _SHLMAIN_
// #include "ShlMain.h"
// #endif

#ifndef _AVSSHELL_
#include "AvsShell.h"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef SOM_ODDocument_xh
#include <Document.xh>
#endif

#ifndef SOM_ODContainer_xh
#include <ODCtr.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
#endif

#if ODDebug
#ifndef _MEMDEBG_
#include <MemDebg.h>
#endif
#endif

#ifndef __STDIO__
#include <stdio.h>
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"  // Adkins -- added (note GetOutput modeis in here now, not Except.h...)
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifdef _PLATFORM_OS2_
// in \opendoc\src\opendoc\docshell in Feb13 build
// #include "debug.h"
#include <odregapi.h>
#include "odtypesf.h"  //119388 @ST
#endif

#include <builtin.h>

#ifdef _UNICODE_
//************************************
//*   set up for UNICODE             *
//*   8/28/95     jss                *
//************************************

#include <unidef.h>
#include <uconv.h>
#include <avuni.h>
#endif
//=============================================================================
// Constants
//=============================================================================

#define DONTNEEDSYSTEMPROCESS

#ifdef _PLATFORM_OS2_
#define charmacro(a,b,c,d) ((int)a) + ((int)b)*256 + ((int)c)*256*256 + ((int)d)*256*256*256

const OSType      kXMPBentoOSType         =  charmacro('b','n','t','o');
const OSType      kXMPQuestionOSType      =  charmacro('?','?','?','?');
const OSType      kXMPAsteriskOSType      =  charmacro('*','*','*','*');
const ODSShort    kOnePageWidth = 600;
const ODError kUnknownDocumentTypeError  =  1001;

#endif

const ODSShort   kSuspendResumeMessage = 0x01;  // High byte suspend/resume event
const ODSShort   kResumeMask           = 0x01;  // Resume vs. suspend mask

const ODSShort    kUseSpecificScript = 0x1c;

const  ODSShort  kMaxFileNameSize    = 64;


// from DrawDef.h
#define kODKindTestDraw "Apple:Kind:TestDraw"

// Local error codes:

#ifndef kODErrClosingNonODWindow
#define    kODErrClosingNonODWindow  5001
#endif

#if ODDebug
const ODMenuID kDebugMenuID = 100;
// Command IDs for the debugging menu:
enum {
  kODCommandDBMemValidation = 980,
  kODCommandDBHeapChecking,
  kODCommandDBLogStdout,
  kODCommandDBLogDebugWindow,
  kODCommandDBSOMTrace
};
#endif

//=============================================================================
// Local Macros
//=============================================================================

#ifdef ODDebug
#define SHLDebugStr(x)    DebugStr(x)
#else
#define SHLDebugStr(x)
#endif

//=============================================================================
// Static variables
//=============================================================================

#if ODDebug
static ODBoolean gMemValidation = kODFalse;
static ODBoolean gHeapChecking  = kODFalse;
#endif

#ifdef _PLATFORM_OS2_

//=============================================================================
// Global Variables
//=============================================================================
HAB hab;        /* anchor block */
HMQ hmq;        /* PM message queue */

#ifdef _UNICODE_
//************************************
//*   set up for UNICODE             *
//*   8/28/95     jss                *
//************************************

char      szMsg1[CCHMAXPATH];
char      szMsg2[CCHMAXPATH];
#endif

//==============================================================================
// Function Prototype
//==============================================================================

extern "C" {
MRESULT EXPENTRY ODShellWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
}

#endif  //_PLATFORM_OS2_

ODFileSpec CreateUntitledFileSpec(const char * TheFileName);

#ifdef _PLATFORM_OS2_
static ODBoolean isValidBento( FILE *fd );
#endif

//=============================================================================
// Static Function
//=============================================================================

static void GetCtrDocTopDraft(Environment* ev,
                              ODSession* session,
                              PlatformFile* file,
                              ODContainer** container,
                              ODDocument** document,
                              ODDraft** draft)
{
  ODFileSpec lvalue = file->GetFileSpec();

  *container = GetFileContainer(ev, session, &lvalue);
  *document = (*container)->AcquireDocument(ev,kODDefaultDocument);
  *draft = (*document)->AcquireDraft(ev,kODDPReadOnly,0,kODNULL,kODPosTop,kODFalse);
}


#ifdef _PLATFORM_OS2_

char* GetPathName (char* docName)
{
    char   drive[3] = "::";
    char*  hostName;
    char*  pathName;
    APIRET rc = NO_ERROR;
    char*  tmp;
    ULONG  ulOrdinal = 0;

    pathName = (char*)SOMMalloc(strlen(docName)+1);
    if (strstr(docName, "LOCALHOST") == docName)
    {
      strcpy(pathName, docName+10);
      return pathName;
    }

    if (hostName = getenv("HOSTNAME"))
    {
      if (strstr(docName, strupr(hostName)) == docName)
      {
        strcpy(pathName, docName+strlen(hostName)+1);
        return pathName;
      }
    }

    for (char c = 'C'; c <= 'Z'; c++)
    {
      drive[0] = c;

      {
        BYTE        fsqBuffer[sizeof(FSQBUFFER2) + (3 * CCHMAXPATH)] = {0};
        PFSQBUFFER2 pfsqBuffer = (PFSQBUFFER2)fsqBuffer;
        ULONG       cbBuffer = sizeof(fsqBuffer);
        PBYTE       prgFSAData = NULL;
        PBYTE       pszFSDName = NULL;

        rc = DosQueryFSAttach(drive,
                              ulOrdinal,
                              FSAIL_QUERYNAME,
                              pfsqBuffer,
                              &cbBuffer);

        if (rc == NO_ERROR)
        {
          pszFSDName = (BYTE*)(pfsqBuffer->szName + pfsqBuffer->cbName + 1);
          prgFSAData = strupr(pszFSDName + pfsqBuffer->cbFSDName + 1);

          // Only consider NFS remote file
          if ((pfsqBuffer->iType == FSAT_REMOTEDRV) &&
              (strcmp(pszFSDName, "NFS") == 0))
          {
            if (strstr(docName, prgFSAData) == docName)
            {
              pathName = (char*)malloc(strlen(docName) + 3);
              pathName[0] = c;
              pathName[1] = ':';
              pathName[2] = '\0';
              strcat(pathName, docName+strlen(prgFSAData));
              return pathName;
            }
          }
        }
      }
    }

    // Default is pathName = docName
    strcpy(pathName, docName);
    return pathName;
}
const ULONG kBUFFER_SIZE = 2048;

ODBoolean VerifyKind(ODISOStr kind)
{
   char pBuffer[kBUFFER_SIZE];    // 119227
   ULONG size = kBUFFER_SIZE;
   OSErr err = 0;

   if(!kind[0]) return kODFalse; // [123673]
   int rc = ODQueryPartHandlerList( kind, pBuffer, (ODULong) &size);

   if(!size)
   {
           return kODFalse;   // [123673] : don't throw here !

#ifdef _UNICODE_

            WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                           AV_PARTREGERROR, sizeof(szMsg1), szMsg1);

            WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                           AV_ERRORTITLE, sizeof(szMsg2), szMsg2);

            WinMessageBox(HWND_DESKTOP,
                                 HWND_DESKTOP,
                                 szMsg1,
                                 szMsg2,
                                 0L,
                                 MB_OK | MB_CRITICAL);
#else
            WinMessageBox(HWND_DESKTOP,
                                 HWND_DESKTOP,
                                 "Part Registration Information Not Found!",
                                 "Error!",
                                 0L,
                                 MB_OK | MB_CRITICAL);
#endif // _UNICODE_

           THROW(err);
   }

//   SOMFree(pBuffer);
   return kODTrue;
}
#endif

//=============================================================================
// AvsShell
//=============================================================================


//-----------------------------------------------------------------------------
// Initialization
//-----------------------------------------------------------------------------

AvsShell::AvsShell()
{
  fSOMEnvironment     = kODNULL;
  fSession         = kODNULL;
  fContainer         = kODNULL;
  fDocument         = kODNULL;
  fDraft           = kODNULL;
  fSurrogateContainer = kODNULL;
  fSurrogateDocument  = kODNULL;
  fSurrogateDraft     = kODNULL;
  fAvailabilityServer = kODNULL;
  fPermissions      = kODDPNone;
  fPlatformWindow = kODNULL;  // @res
  fDraftIsSaved = kODFalse;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

AvsShell::~AvsShell()
{
  if (fDraft!=kODNULL)
  {
    ODFinalReleaseObject(fSOMEnvironment,fDraft);
    ODFinalReleaseObject(fSOMEnvironment,fDocument);
    ODFinalReleaseObject(fSOMEnvironment,fContainer);
    ODFinalReleaseObject(fSOMEnvironment,fSurrogateDraft);
    ODFinalReleaseObject(fSOMEnvironment,fSurrogateDocument);
    ODFinalReleaseObject(fSOMEnvironment,fSurrogateContainer);
    ODDisposeHandle(fSurrogateContainerHandle);
  }

  delete fAvailabilityServer;
  delete fSession;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void AvsShell::Initialize(char* name /* filename of doc and alias of server */)
{

  // 1. Get SOM
  // 2. Create a session, using class AsSession (like ShellSession, but talks to me)
  // (deleted) 3. Get system facilities from that session
  // 4. Tell the session how to talk to me.
  // (deleted) 5. Install menus
  // 6. Create surrogate Container, Document, and Draft
  // 7. Create availability server
  // 8. Initialize windows (not in delayed-start version)

  // 1.
  fSOMEnvironment = somGetGlobalEnvironment();
  fPermissions = kODDPExclusiveWrite;

  // 2.
  fSession = new AVSShellSession;
  fSession->InitAvailServerSession(fSOMEnvironment, name);

  // 3.

  // 4.
  fSession->SetShellPointer(fSOMEnvironment, this);

  // 5.

  // 6.
   TRY
      fSurrogateContainerHandle = ODNewHandle(0);
      fSurrogateContainer = CreateMemoryContainer(
                              fSOMEnvironment,
                              fSession,
                              fSurrogateContainerHandle,
                              kODBentoMemoryContainer);
      fSurrogateDocument = fSurrogateContainer->
                             AcquireDocument(fSOMEnvironment,
                                             kODDefaultDocument);
      fSurrogateDraft = fSurrogateDocument->
                          AcquireBaseDraft(fSOMEnvironment, kODDPExclusiveWrite);
   CATCH_ALL
      RERAISE;
   ENDTRY

   // 7.
   // the availability server is useless until InitAvailServer is called
   fAvailabilityServer = new IODAvailServer;

   // 8.
}

//-----------------------------------------------------------------------------
// Main Entry Point
//-----------------------------------------------------------------------------

void AvsShell::go( int argc, char *argv[] )
{
    char *    odcfgVariable  = "ODCFG";
    PCSZ      odcfgValue     = 0;
    const     kMaxBufferSize = 1024;
    char *    fileNameIn     = "avlsrvr.bto";
    ODBoolean printing       = kODFalse;                 //  ced [1174742]
    char      completeFileName[kMaxBufferSize];
    char      completeBkpFileName[kMaxBufferSize];
    char      kindName[120];
    char      asName[256];

    //check environment where opendoc odcfg directory is ("etc") Note: no trailing '\' or ';'
    if ( !DosScanEnv( odcfgVariable, &odcfgValue) )
    {
      if (odcfgValue)                                         // If ODCFG is set
      {
        strcpy(completeFileName, odcfgValue);                 // Use it as the path
        strcat(completeFileName, "\\");                       // Add a trailing '\'
      }
    }
    // Note that if ODCFG not set or error above, then completeFileName is still empty and we don't use a path.

    strcat( completeFileName, fileNameIn);

    //create fully qualified name for the backup file
    //replace ".bto" with ".bak"
    strcpy(completeBkpFileName,completeFileName);                // Start with same path, file name, and extension
    completeBkpFileName[strlen(completeBkpFileName)-3] = '\0';   // Remove "bto" extension
    strcat(completeBkpFileName,"bak");                           // Append "bak" extension

    char* asSuffixName = getenv("AVLSVRNAME") ? getenv("AVLSVRNAME") : "AS";
    strcpy(asName, getenv("HOSTNAME"));
    strcat(asName, "_");
    strcat(asName, asSuffixName);

    TRY
      // this->Initialize();  wait until we've parsed the input
      // hab and hmq needed to be defined here so that subsequent call
      // to WinMessageBox will bring up the message box!
      hab = WinInitialize(0);
      hmq = WinCreateMsgQueue( hab, 0 );

      PlatformFile* file = new PlatformFile;
      kindName[0] = 0;

      int argnum = 1;
      while (argnum < argc) {
        char * thisarg =  argv[argnum];
        if (thisarg[0]=='-')
        {
          switch (thisarg[1])
          {
            case 'f':
            case 'F':
              if (strlen(thisarg+2) > 0) fileNameIn = thisarg+2;
              break;
          } /* endswitch */
        }
        else if (thisarg[0]==0)
             {
                 // weird! specified an empty string.  Ignore
             }
             else
             {
                // Do nothing here:
                // AvsShell.exe ignore other arguments other that -f -t -s
             } /* endif */
        argnum++;
      } /* endwhile */

      ODBoolean existsFile;
      if ( fileNameIn)
      {
        {
          // Presumed nonexistant until proven otherwise
          existsFile = kODFalse;
          FILE *fp = fopen(completeFileName, "rb "); //(129464)
          if ( fp )
          {
            // fileNameIn exists, but possibly invalid
            existsFile = kODTrue;

            if (!isValidBento( fp ) )
            {
              // Close completeFileName since it's an invalid bento file
              fclose(fp);

              // Open the backup bento file
              fp = fopen(completeBkpFileName, "rb");

              // Check if backup bento file is valid
              if (fp && isValidBento(fp))
              {
                // Copy the backup bento file to completeFileName
                APIRET rc = DosCopy(completeBkpFileName, completeFileName, DCPY_EXISTING);
                if (rc != NO_ERROR)
                {

                  // DosCopy returns an error

#ifdef _UNICODE_
//************************************
//*   set up for UNICODE             *
//*   8/28/95     jss                *
//************************************

                  WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                                 AV_CANTRESTOREBTO, sizeof(szMsg1), szMsg1);

                  WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                                 AV_ERRORTITLE, sizeof(szMsg2), szMsg2);

                  WinMessageBox(HWND_DESKTOP,
                                HWND_DESKTOP,
                                szMsg1,
                                szMsg2,
                                0L,
                                MB_OK | MB_ERROR);
#else
/*
                  WinMessageBox(HWND_DESKTOP,
                                HWND_DESKTOP,
                                "Cannot restore avlsrvr.bto!",
                                "Error!",
                                0L,
                                MB_OK | MB_ERROR);
*/
                    fclose(fp);
                    DosDelete(completeBkpFileName);
                    DosDelete(completeFileName);
                    existsFile = kODFalse;

#endif
//                  return;
                }
                else
                {
                  // DosCopy ok
                  fclose(fp);
                }
              }
              else
              {
                // Backup bento file does not exist or is invalid

#ifdef _UNICODE_
//************************************
//*   set up for UNICODE             *
//*   8/28/95     jss                *
//************************************

                WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                               AV_BTOBAKCORRUPT, sizeof(szMsg1), szMsg1);

                WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                               AV_ERRORTITLE, sizeof(szMsg2), szMsg2);

                WinMessageBox(HWND_DESKTOP,
                              HWND_DESKTOP,
                              szMsg1,
                              szMsg2,
                              0L,
                              MB_OK | MB_ERROR);
#else
/*
                WinMessageBox(HWND_DESKTOP,
                              HWND_DESKTOP,
                              "Avlsrvr.bto and Avlsrvr.bak are both corrupted!\nDelete them and restart AVSSHELL.EXE",
                              "Error!",
                              0L,
                              MB_OK | MB_ERROR);
*/

                  fclose(fp);
                  DosDelete(completeBkpFileName);
                  DosDelete(completeFileName);
                  existsFile = kODFalse;
#endif
//                return ;
              } //endif
            }
            else
            {
              // Back up the bento file for avsshell
              APIRET rc = DosCopy(completeFileName, completeBkpFileName, DCPY_EXISTING);
              fclose(fp);
            } /* endif */
          } /* endif */
        }
      } /* endif */

      if (completeFileName && existsFile)        //(129464)
      {
        ODFileSpec fileSpec;
        strcpy((char*)(fileSpec.name), completeFileName); //129464)
        this->Initialize(fileNameIn);
        file->Specify( &fileSpec );
        this->OpenFile( file);
        this->Open();
        fSession->StartServer(fSOMEnvironment, asName);
        this->OpenDraft(fDraft);
      }
      else
      {
        // this is the normal startup when
        // the AS is started for the first time from an empty file
        ODFileSpec fileSpec;
        char * fileNameUsed = completeFileName;  //129464

        this->Initialize(fileNameUsed);
        file = this->OpenUntitled( fileNameUsed );
        this->Open();
        fSession->StartServer(fSOMEnvironment, asName);
        this->OpenDraft(fDraft);
      } /* endif */

    CATCH_ALL
      DebugStr("ODShell: Initialize() failed");
      return;
    ENDTRY

      QMSG qmsg;                           /* Message from message queue   */
      if (!printing) {                      // ced [117472]
        while( WinGetMsg( hab, &qmsg, 0L, 0, 0 ) )
          WinDispatchMsg( hab, &qmsg );
        if (!fDraftIsSaved) this->SaveDraft();
      }
      ODReleaseObject(fSOMEnvironment,fDraft);
      ODReleaseObject(fSOMEnvironment,fDocument);
      ODReleaseObject(fSOMEnvironment,fContainer);

      WinDestroyMsgQueue( hmq );           /* Tidy up...                   */
      WinTerminate( hab );                 /* Terminate the application    */
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

MRESULT EXPENTRY ODShellWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   switch (msg) {
     case WM_PAINT:
     {
       RECTL rcl;
       HPS hps = WinBeginPaint(hwnd, NULLHANDLE, &rcl);
       WinFillRect(hps, &rcl, CLR_WHITE);
       WinEndPaint(hps);
       return 0;
     }

     case WM_CLOSE:
     {
       AvsShell* theShell = (AvsShell *)WinQueryWindowPtr(hwnd, 0);
       theShell->SaveDraft();
       WinPostQueueMsg( hmq, WM_QUIT, 0, 0);
       break;
     }
   }
   return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int QueryFilePath( ODFileSpec fspec)
{
   FILESTATUS3 fs3;
   APIRET err;
   err = DosQueryPathInfo( (PSZ)fspec.name
                                    , /*infolevel:std*/ FIL_STANDARD
                                    , &fs3
                                    , sizeof(fs3)
                                    );
   return err;  // if an error occurs, then let's assume we're supposed
                 // to say the file doesn't exist
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

ODFileSpec  AvsShell::CreateNamedContainer(ODDocument** documentPtr,
                                           ODContainer** containerPtr,
                                           PlatformFile* currentFile,
                                           const char * basename)
{
   PlatformFile*    file;
   HMTX             hmtx;

   //<crs>-11/15/94-protect file creation with a mutex sem.

#ifdef _UNICODE_
//************************************
//*   set up for UNICODE             *
//*   8/28/95     jss                *
//************************************

   WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                  AV_SEMUNTITLED, sizeof(szMsg1), szMsg1);

   APIRET rc = DosCreateMutexSem( szMsg1, &hmtx,
                        DC_SEM_SHARED, 0 );
#else
   APIRET rc = DosCreateMutexSem( "\\SEM32\\UNTITLED", &hmtx,
                        DC_SEM_SHARED, 0 );
#endif


   if ( rc == ERROR_DUPLICATE_NAME )
#ifdef _UNICODE_
      rc = DosOpenMutexSem( szMsg1, &hmtx );
#else
      rc = DosOpenMutexSem( "\\SEM32\\UNTITLED", &hmtx );
#endif

#ifdef _UNICODE_
   WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                  AV_CANNOTOPENSEM, sizeof(szMsg1), szMsg1);
   WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                  AV_CANNOTGETSEM, sizeof(szMsg2), szMsg2);

   WASSERTM( rc == NO_ERROR, szMsg1 );

   rc = DosRequestMutexSem( hmtx, SEM_INDEFINITE_WAIT );
   WASSERTM( rc == NO_ERROR, szMsg2 );
#else
//   WASSERTM( rc == NO_ERROR, "Can't open 'Untitled' sempahore" );

   rc = DosRequestMutexSem( hmtx, SEM_INDEFINITE_WAIT );
//   WASSERTM( rc == NO_ERROR, "Can't get 'Untitled' sempahore" );
#endif
   ODFileSpec myTSpec = currentFile->GetFileSpec();
   file = new PlatformFile();

   strcpy( (char*)(myTSpec.name), basename);

   file->Specify(&myTSpec);
   file->Create(kODShellSignature, kODShellSignature, 0);

  (*containerPtr) = CreateFileContainer(fSOMEnvironment, fSession, &myTSpec );
  if (documentPtr)
    (*documentPtr) = (*containerPtr)->AcquireDocument(fSOMEnvironment,kODDefaultDocument);

   //<crs>-12/06/94-Let the workplace know it is an OpenDoc file.
//   file->Open();

#ifdef _UNICODE_
//************************************
//*   set up for UNICODE             *
//*   8/28/95     jss                *
//************************************

   WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                  AV_OPENDOCDOCUMENT, sizeof(szMsg1), szMsg1);
   file->SetEAFromName( ".TYPE", szMsg1 );
#else
//   file->SetEAFromName( ".TYPE", "OpenDoc Document" );
#endif
//    file->Close();

   rc = DosReleaseMutexSem( hmtx );
   rc = DosCloseMutexSem( hmtx );

   return myTSpec;
}

//-----------------------------------------------------------------------------
// Open
//-----------------------------------------------------------------------------

void  AvsShell::OpenFile(PlatformFile*  file)
{
// Comments added: /******** strom@watson ********/
// This function is called if a file already exists.
// The job of this function is to obtain the top draft
// of the document represented by this file.
// The side effect is to initialize fContainer, fDocument, fDraft,
// and fIsNew.

  GetCtrDocTopDraft(fSOMEnvironment,
                    fSession,
                    file,
                    &fContainer,
                    &fDocument,
                    &fDraft);
  fDraft = fDocument->AcquireDraft(fSOMEnvironment,
                                   fPermissions,
                                   0,
                                   fDraft,
                                   kODPosSame,
                                   kODTrue);
  fIsNew = kODFalse;   /******** strom@watson ********/
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

PlatformFile* AvsShell::OpenUntitled(const char *FileName)
{
  // Comments added: /******** strom@watson ********/
  // create a container, get the draft and set the DraftProperties
  // Despite its name, OpenUntitled creates a new Document file, regardless of
  // whether it is untitled or not.
  // A side effect is to initialize fDocument, fContainer, fDraft, and fIsNew
  // An invisible side effect is to initialize fDraft's DraftProperties
  // to include editor kind and editor name.  These properties will be
  // readable later by Open.

  // res: In the memory-streamlined version, these properties will be
  // ignored later by Open.

  PlatformFile* currentFile = new PlatformFile;

  ODFileSpec newFileFSP = CreateNamedContainer(&fDocument,
                                               &fContainer,
                                               currentFile,
                                               FileName);
  currentFile->Specify( &newFileFSP);

  fDraft = fDocument->AcquireBaseDraft(fSOMEnvironment, fPermissions);
  ODStorageUnit* su = fDraft->AcquireDraftProperties(fSOMEnvironment);
  fDraft->Externalize(fSOMEnvironment);
  ODReleaseObject(fSOMEnvironment, su );

  fIsNew = kODTrue;

  return ( currentFile );
}

//-----------------------------------------------------------------------------
// The following comments are from /******** strom@watson ********/

// Open is called after the last saved draft has been loaded or
// a new draft has been created.

// The function of Open is:
// 1. to establish the visible draft:
//   if the document is read-only, the visible draft is the current draft.
//   if the document is read-write, a new draft should be created
//      on top of the current draft, and this becomes the visible draft.
//      Also the LinkService should be opened.
//
// 2. to bring up the root part, either by
// creating it if it hasn't yet been created, or by asking the
// draft to internalize it if it has already been created.
// res: In the streamlined version, we do not bring up any
// root part.  Instead, we merely create a platform window
// (which the root part would have done) We don't create
// an ODWindow from this platform window, nor do we store it
// in the session, or save its open state in the draft or
// any of those things that normal documents do.
//
// In the delayed-start version, we defer this step until
// window-initialization ("OpenDraft") time

// 3. to perform whatever window management is required by the platform.
//    res: In the streamlined version, we just do a WinShowWindow
//         on the platform window to make it visible.  We don't
//         in particular use any objects of class WindowState.
//         And in the delayed-start version, we defer this step
//         until window-initialization ("OpenDraft") time
//-----------------------------------------------------------------------------

void  AvsShell::Open()
{
  if (fDraft==kODNULL) return;

  if (this->HasWriteAccess()) {
    fDraft = fDocument->CreateDraft(fSOMEnvironment,fDraft,kODTrue);

    /******** strom@watson ********/
    ODFileSpec documentFile = GetPlatformFileFromContainer(
                                fSOMEnvironment, fContainer)->GetFileSpec();
  }


  // Comments added by // strom@watson
  // In the previous implementation, the true branch never
  // executed for brand new files because the functionality
  // was duplicated in OpenUntitled.  Now this code will
  // execute for new files, creating the root part.


  fAvailabilityServer->InitAvailServer(fSOMEnvironment,
                                       fIsNew,
                                       this,
                                       fDraft,
                                       fSurrogateDraft);
}

//-----------------------------------------------------------------------------
// this doesn't get called in the streamlined version
// but it does get called in the delayed-start version
//
// internalize the windowstate and open windows of fDraft
// In the delayed-start version, all the window-related stuff
// goes here.  Delayed-start is only used with streamlined;
// therefore there is no ODWindow object, only PM Windows:
// The steps are as follows:
// 1. Initialize the PM Window system
//    Since the DSOM server is already running, this can no longer
//    deadlock with a docshell process which is trying to
//    auto-start the avsshell while handling a PM event.
// 2. Register a PM class
// 3. Create a platform window
// 4. show the platform window on the screen
//-----------------------------------------------------------------------------

void  AvsShell::OpenDraft(ODDraft* draft)
{
//---<1>
     hab = WinInitialize(0);
     hmq = WinCreateMsgQueue( hab, 0 );

//---<2>
     WinRegisterClass(                /* Register window class        */
        hab,                          /* Anchor block handle          */
        (PSZ)"OpenDocShell",          /* Window class name            */
        (PFNWP)ODShellWndProc,        /* Address of window procedure  */
        CS_SIZEREDRAW,                /* Class style                  */
        4);                           /* Extra window words           */
                                      /* 119471 [pfe] */
//---<3>
    fPlatformWindow = fSession->CreatePlatformWindow(fSOMEnvironment, ODPlatformWindowDefaultCreateOptions);

//---<4>
   WinShowWindow(fPlatformWindow, FALSE);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void AvsShell::SaveDraft()
{
  // externalize the draft
  if ( this->HasWriteAccess() && fDraft->ChangedFromPrev(fSOMEnvironment) )
  {
    fDraft->Externalize(fSOMEnvironment);
    fDraftIsSaved = kODTrue;
  }
}

//-----------------------------------------------------------------------------
// Drafts
//-----------------------------------------------------------------------------

ODBoolean  AvsShell::HasWriteAccess()
{
// returns whether draft was open w/ write access
  return fPermissions>=kODDPSharedWrite;
}

//-----------------------------------------------------------------------------
// Availability Server access
//-----------------------------------------------------------------------------
  // CloneLinkSource:
  // Called by availability server when a linkID needs to be opened.
  // The pathName is used to find the document.  The
  // document is opened read-only.  The linkservice is
  // asked to find the SU of the LinkSource corresponding to this linkID.
  // This LinkSource is then cloned into the surrogateDraft,
  // and the SUID of that clone is returned.

ODStorageUnitID AvsShell::CloneLinkSource(ODLinkID linkID,
                                           ODISOStr docName)
{
// 1. Open up a temporary container, document, and (R/O) draft for the file in pathName
// 2. Call LinkManager::GetLinkSrcInDraft to find this linkID in this draft
// 3. Clone the linksource from the temporary draft to the surrogate draft
// 4. Release the temporary container, document, and draft

// 1.
  PlatformFile* tempFile = new PlatformFile;
  ODFileSpec tempFileSpec;
  ODContainer* tempContainer = kODNULL;
  ODDocument* tempDocument = kODNULL;
  ODDraft* tempDraft = kODNULL;
  ODStorageUnitID lsSUID;
  ODStorageUnitID clonedLsSUID;
  ODDraftKey key;

  //tempFileSpec.name = kODNULL;
  TRY

    char* pathName = GetPathName(docName);
    if (_access(pathName, 00) == -1)
    {
      SOMFree(pathName);
      THROW(kODErrInvalidDocPathName);
    }

    strcpy((char*)(tempFileSpec.name), pathName);
    SOMFree(pathName);

    tempFile->Specify(&tempFileSpec);
    GetCtrDocTopDraft(fSOMEnvironment,
                      fSession,
                      tempFile,
                      &tempContainer,
                      &tempDocument,
                      &tempDraft);


  // 2.
  lsSUID = fSession->
             GetLinkService(fSOMEnvironment)->
               GetLinkSrcInDraft(fSOMEnvironment, tempDraft, linkID);

  // 3.
  key = tempDraft->BeginClone(fSOMEnvironment, fSurrogateDraft, kODNULL, kODCloneAll);
  clonedLsSUID = tempDraft->Clone(fSOMEnvironment, key, lsSUID, 0, 0);

  // 27480
  // Use GetWeakSURefProp and SetWeakSURefProp to obtain values.
  {
     TempODStorageUnit tempSU = tempDraft->AcquireStorageUnit(fSOMEnvironment, lsSUID);
     ODID partID = ODGetWeakSURefProp (fSOMEnvironment, tempSU,
                         kODPropSourcePart, kODWeakStorageUnitRef);
     if (partID) {
        ODID newID = tempDraft->Clone(fSOMEnvironment, key, partID, kODNULL, kODNULL);
        TempODStorageUnit tempNewSU = fSurrogateDraft->AcquireStorageUnit(fSOMEnvironment, clonedLsSUID);
        ODSetWeakSURefProp (fSOMEnvironment, tempNewSU,
                     kODPropSourcePart, kODWeakStorageUnitRef, newID);
     }    /* endif */
     else
     PRINT("******** Error no part ID\n");
  }
  tempDraft->EndClone(fSOMEnvironment, key);

  // 4.
  if (tempDraft != kODNULL)
  {
    ODReleaseObject(fSOMEnvironment,tempDraft);
    ODReleaseObject(fSOMEnvironment,tempDocument);
    ODReleaseObject(fSOMEnvironment,tempContainer);
  }
  delete tempFile;
  //if (tempFileSpec.name != kODNULL) delete tempFileSpec.name;
  return(clonedLsSUID);

  CATCH_ALL
    if (tempDraft != kODNULL)
    {
      ODReleaseObject(fSOMEnvironment,tempDraft);
      ODReleaseObject(fSOMEnvironment,tempDocument);
      ODReleaseObject(fSOMEnvironment,tempContainer);
    }
    delete tempFile;

    RERAISE;
    return(0);

  ENDTRY

}

/*
 *=============================================================================
 *=============================================================================
 */

void AvsShell::SetDocListHWND(HWND docListHWND)
{
  fDocListHWND = docListHWND;
}

/*
 *=============================================================================
 *=============================================================================
 */

void AvsShell::UpdateDocList(ULONG docID,
                            char* pathName,
                            char flag)
{
    char item[512];
    time_t ltime;
    char stime[26];

    if (!getenv("AVSSHELL_UPDATE"))
      return;

    time(&ltime);
    sprintf(stime, ctime(&ltime));
    stime[24] = '\0';
    memset(item, 0, 512);

    switch(flag)
    {
      case 'o':
        sprintf(item, "DocID [%d] named [%s] is opened on [%s]", docID, pathName, stime);
        break;
      case 'c':
        sprintf(item, "DocID [%d] named [%s] is closed on [%s]", docID, pathName, stime);
        break;
      case 'r':
        sprintf(item, "DocID [%d] named [%s] is removed on [%s]", docID, pathName, stime);
        break;
    }

    HMQ tmpq = WinCreateMsgQueue(hab, 0);
    if (tmpq != NULLHANDLE)
    {
      WinSendMsg(fDocListHWND,
                 LM_INSERTITEM,
                 MPFROMSHORT(LIT_END),
                 MPFROMP(item));
      WinDestroyMsgQueue(tmpq);
    }
}

/*
 *=============================================================================
 *=============================================================================
 */

void AvsShell::SetLinkListHWND(HWND linkListHWND)
{
  fLinkListHWND = linkListHWND;
}

/*
 *=============================================================================
 *=============================================================================
 */

void AvsShell::UpdateLinkList(ULONG linkID,
                             ULONG docID,
                             ULONG flag)
{
    char item[512];
    time_t ltime;
    char stime[26];

    if (!getenv("AVSSHELL_UPDATE"))
      return;

    time(&ltime);
    sprintf(stime, ctime(&ltime));
    stime[24] = '\0';
    memset(item, 0, 512);

    switch(flag)
    {
      case 0:
        sprintf(item, "LinkID [%d] of DocID [%d] resides on file on [%s]", linkID, docID, stime);
        break;
      case 1:
        sprintf(item, "LinkID [%d] of DocID [%d] resides in document on [%s]", linkID, docID, stime);
        break;
      case 2:
        sprintf(item, "LinkID [%d] of DocID [%d] resides in Link Manager on [%s]", linkID, docID, stime);
        break;
      case 3:
        sprintf(item, "LinkID [%d] of DocID [%d] resides in both document and Link Manager on [%s]", linkID, docID, stime);
        break;
      case 4:
        sprintf(item, "LinkID [%d] of DocID [%d] is removed on [%s]", linkID, docID, stime);
        break;
    }

    HMQ tmpq = WinCreateMsgQueue(hab, 0);
    if (tmpq != NULLHANDLE)
    {
      WinSendMsg(fLinkListHWND,
                 LM_INSERTITEM,
                 MPFROMSHORT(LIT_END),
                 MPFROMP(item));
      WinDestroyMsgQueue(tmpq);
    }
}


#define LBLmagicBytes      0  /* 8 bytes: the magic byte identifier          */
#define LBLflags          8   /* 2        the label flags                    */
#define LBLbufSize       10   /* 2        TOC buffer size / 1024            */
#define LBLmajorVersion   12  /* 2        major format version number        */
#define LBLminorVersion   14  /* 2        minor format version number        */
#define LBLtocOffset     16   /* 4        offset to start of TOC            */
#define LBLtocSize       20   /* 4        total byte size of the TOC        */

#define LBLsize           24  /* defined size of the container label        */

#define MagicByteSequence "\xA4""CM""\xA5""Hdr""\xD7"    /* Must be 8 characters          */

static ODBoolean isValidBento( FILE *fp )
{
   long pos;
   unsigned char label[LBLsize];
   ODBoolean validFile = kODFalse;

   fseek( fp, 0L, SEEK_END );
   pos = ftell( fp ) - sizeof( label );
   if (!fseek( fp, pos, SEEK_SET ))
      if ( fread( &label, 1, sizeof( label ), fp ) == sizeof( label ))
         if ( !( memcmp( &label, MagicByteSequence,
                               strlen( MagicByteSequence ) )))
            validFile = kODTrue;

   return( validFile );
}
