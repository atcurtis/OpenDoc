/* @(#)Z 1.2 os2/src/utils/dualpath.h, odutils, od96os2, odos29646d 96/11/15 15:49:52 (96/08/23 01:44:24) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odutils
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
/* Include to aid dual-pathing of PM applications for 16/32-bit. */
/* Assumes "B16" is defined for the 16-bit case.                 */

/* Specials:                                                     */
/*                                                               */
/*   USLONG  -- USHORT for 16-bit, ULONG for 32-bit              */
/*   SLONG   -- SHORT for 16-bit,  LONG for 32-bit               */
/*   PSLONG  -- PSHORT for 16-bit, PLONG for 32-bit              */
/*                                                               */
/*                                                               */

#if defined(B16)
  /* 16-bit */
  #define USLONG  USHORT
  #define SLONG   SHORT
  #define PSLONG  PSHORT
  #define PUSLONG PUSHORT
  /* Now let us use some 32-bit functions in the 16-bit version */
  /* Event and Mutex semaphore stuff -- replaces RAM semaphores */
  #define HEV unsigned long
  #define DosCreateEventSem(x,y,z,f) if (f) *(y)=0; else *(y)=1;
  #define DosPostEventSem(x)        DosSemClear(&x)
  #define DosResetEventSem(x,y)     DosSemSet(&x)
  #define DosWaitEventSem(x,y)      DosSemWait(&x,(signed long)(y))

  #define DosCreateMutexSem(x,y,z,f) if (f) *(y)=1; else *(y)=0;
  #define DosRequestMutexSem(x,y)   DosSemRequest(&x,(signed long)(y))
  #define DosReleaseMutexSem(x)     DosSemClear(&x)

  /* Misc */
  #define DosDelete(x)              DosDelete(x,0L)
  #define WinQueryWindow(x,y)       WinQueryWindow(x,y,NULL)
  #define WinWindowFromPoint(x,y,z) WinWindowFromPoint(x,y,z,NULL)
  #define BITMAPINFOHEADER2         BITMAPINFOHEADER
  #define BITMAPINFO2               BITMAPINFO
  #define PBITMAPINFO2              PBITMAPINFO
  #define RGB2                      RGB
  #define _timezone                 timezone
  #define RexxStart                 -REXXSAA
  #define RexxVariablePool          RxVar
  #define _ltoa(x,y,z)              ltoa((x),(y),(z))
  #define _itoa(x,y,z)              itoa((x),(y),(z))

#else
  /* 32-bit */
  #define USLONG  ULONG
  #define SLONG   LONG
  #define PSLONG  PLONG
  #define PUSLONG PULONG

  #define DosWaitEventSem(x,y)      DosWaitEventSem((x),(unsigned)(y))
  #define DosRequestMutexSem(x,y)   DosRequestMutexSem((x),(unsigned)(y))

  #undef  NULL
  #define NULL   0

  #define halloc(x,y) malloc((unsigned long)(x)*(unsigned long)(y))
  #define hfree(x)    free(x)

  #define far
  #define near
  #define huge
  #define cdecl
  #define _loadds

  #define ltoa(x,y,z) _ltoa((x),(y),(z))

#endif
