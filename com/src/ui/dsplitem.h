/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odui
 *
 *   CLASSES:   DispatchModuleListItem
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
/* @(#) 1.3 com/src/ui/dsplitem.h, odui, od96os2, odos29646d 10/31/96 12:45:54 [11/15/96 15:51:25] */
/********************************************************************/
/*                                                                  */
/* Copyright (C) Apple Computer, Inc., 1994                         */
/*                                                                  */
/*                                                                  */
/********************************************************************/
#ifndef _DSPLITEM_
#define _DSPLITEM_

/* The following class definitions are used by the IKeySet collection */
/* class in Disptch.cpp.  These must be in a separate include file    */
/* for automatic generation of template functions.                    */

class ODDispatchModule;

class DispatchModuleListItem {
   int key;
   ODDispatchModule *dispmod;
public:
   DispatchModuleListItem(ODDispatchModule* module, int eventData) {
      dispmod = module;
      key = eventData;
   }
   ODDispatchModule* DispatchModule() const { return dispmod; }
   friend int const &key(DispatchModuleListItem const &e) { return e.key; }
};
#endif
