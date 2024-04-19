/* @(#)Z 1.5 com/src/docshell/DocShell.h, odshell, od96os2, odos29646d 96/11/15 15:56:39 (96/10/29 09:23:39) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odshell
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

#ifndef _DOCSHELLH_
#define _DOCSHELLH_

#define SaveEv() \
       ODError savedNonSOMError, savedSOMError; \
       savedSOMError = ODGetSOMException(ev); \
       if (savedSOMError == kODNoError) \
          savedNonSOMError = ErrorCode(); \
       SetErrorCodeEv(ev, kODNoError); 

#define RestoreEv() \
       if (savedSOMError == kODNoError) \
          SetErrorCodeEv(ev, savedNonSOMError); \
       else \
          ODSetSOMException(ev, savedSOMError);

#define RestoreApplModal(hwnd) \
    { \
       SaveEv() \
       try \
       { \
          EnableApplModal(ev, hwnd, TRUE); \
       } \
       catch (ODException _exception) \
       { \
          SetErrorCodeEv(ev, kODNoError); \
       } \
       RestoreEv() \
    }

#define SaveAndRestoreEv1(object1) \
    { \
       SaveEv() \
       try \
       { \
          ODReleaseObject(ev, object1); \
       } \
       catch (ODException _exception) \
       { \
          SetErrorCodeEv(ev, kODNoError); \
       } \
       RestoreEv() \
    } 

#define SaveAndRestoreEv2(object1, object2) \
    { \
       SaveEv() \
       try \
       { \
          if (object1) \
             ODReleaseObject(ev, object1); \
          if (object2) \
             ODReleaseObject(ev, object2); \
       } \
       catch (ODException _exception) \
       { \
          SetErrorCodeEv(ev, kODNoError); \
       } \
       RestoreEv() \
    }

#define SaveAndRestoreEv3(object1, object2, object3) \
    { \
       SaveEv() \
       try \
       { \
          if (object1) \
             ODReleaseObject(ev, object1); \
          if (object2) \
             ODReleaseObject(ev, object2); \
          if (object3) \
             ODReleaseObject(ev, object3); \
       } \
       catch (ODException _exception) \
       { \
          SetErrorCodeEv(ev, kODNoError); \
       } \
       RestoreEv() \
    }
#endif
