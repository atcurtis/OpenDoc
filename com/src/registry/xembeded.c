/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odconfig
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
/* @(#) 1.3 com/src/registry/xembeded.c, odconfig, od96os2, odos29646d 10/31/96 12:45:13 [11/15/96 15:49:47] */
/*
  File:    Xembeded.c

  Contains:  Handlers for Registry Bento Calls

  Written by:  Kirk Searls

  IBM Change History (most recent first):

     <1>   5/03/95  jso   120604: added check for Endian neutrality

  To Do:
*/
/* (use tabs = 2 to view this file correctly) */
/*---------------------------------------------------------------------------*
 |                                                                           |
 |                     <<<        XEmbeded.c         >>>                     |
 |                                                                           |
 |         Example Container Manager Handlers for Embedded Containers        |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  1/8/92                                   |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1992                    |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 This file contains a fully documented example of a set of handlers and its metahandler
 as it would be used by the Container Manager for embedded containers.  Unlike a non-
 embedded container example (   XHandlrs.c    ), the "I/O" done here is in terms of
 Container Manager API calls (this is a bit incestuous).

 Note, an example of a non-embedded container handler set is defined in the file
 ExampleContainer.c.  It is recommended that that file be read before that file since the
 handling of embedded containers is simply a "permutation" of the non-embedded case.  There
 are also references to the routines in ExampleContainer.c in the documentation presented
 here.

 This example is a complete working set of handlers for embedded containers.  Such handers
 do all "I/O" in terms of API calls for a value.  Thus this example is fully general to
 support all forms of embedded containers!  As with    XHandlrs.c     the only changes that
 may be needed are for error reporting and program termination.

 Read the documentation for these routines in the order presented.  They have been arranged
 so that this documentation reads as a tutorial on how to write a typical set of embedded
 container handlers.
*/


/*------------------------------------------------------------------------------------*
 | DOS (80x86) USERS -- USE THE "LARGE" MEMORY MODEL WHEN COMPILED FOR 80X86 MACHINES |
 *------------------------------------------------------------------------------------*

 The Container Manager is built with this same option and assumes all handler and
 metahandler interfaces are similarly built and can be accessed with "far" pointers.
*/

#ifdef _PLATFORM_OS2_

#ifdef _UNICODE_
#define INCL_WIN
#include <os2.h>
#endif

#endif // _PLATFORM_OS2_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "CMAPI.h"
#include "XEmbeded.h"

#ifdef _PLATFORM_OS2_
#include <ODPagtun.h>
#endif

#ifdef _UNICODE_
char szMsg1[Str255];
#endif

                                  CM_CFUNCTIONS

/* See comments in      CMAPIEnv.h      about CM_CFUNCTIONS/CM_END_CFUNCTIONS (used      */
/* only when compiling under C++).                                                      */


/*
 Passed to every handler except for "open" is a "reference constant" or just "refCon" for
 short. This is a general user defined value that allows the user to associate a particular
 container with its handlers.

 The "open" is unique in that it returns the refCon to the Container Manager so that it can
 pass it to all the other handlers.  In order for the "open" itself to know what it is to
 do, the "moral equivalent" of a refCon is passed.  In the API this is referred to as
 "attributes".  The attributes are passed to CMOpen[New]Container() who in turn pass it
 through to the "open".  The open then uses the attributes to associate it with a refCon to
 be used from that point on.

 The Container Manager only transmits attributes and refCons.  The Container Manager itself
 does not know or care what they mean.

 In this embedded container example, we define the refCon that is a pointer to a allocated
 struct with the following definition:                                                  */

 struct MyRefCon {                                /* Our "refCon" layout:                */
   CMSession     sessionData;                      /*    ptr to current container session*/
   CMValue       parentValue;                      /*     the embedded value being created*/
   FILE          *traceFile;                      /*    tracing output file              */
   unsigned long position;                        /*     current position within value    */
   unsigned long size;                            /*    value size                      */
   char          tracing;                          /*    tracing is on (1) or off (0)    */
   char          typeName[1];                      /*    start of container type name    */
 };
 typedef struct MyRefCon MyRefCon, *MyRefConPtr;                                        /*

 The current session data pointer is saved.  It is needed to allocate and free the refCon
 and to report errors through the session handlers.

 The example handlers are for embedded containers.  Unlike non-embedded containers, a
 value (CMValue) of a parent container is to be given the entire embedded container as its
 "value".  This means that the "I/O" calls for an embedded container are actually API
 calls to "write" or "read" the parent's value.

 A CMValue here thus serves the same purpose as a FILE* variable in the non-embedded case.
 There the FILE* variable is used in all stream I/O calls.  Here the CMValue is used in all
 embedded container "I/O" calls which take the form of API calls back to the Container
 Manager itself.  Since the value corresponds to one in the parent of the embedded
 container, i.e., a non-embedded container, then the calls back to the API will translate
 into appropriate calls to the parent's handlers.  That will end up doing the physical I/O
 to the container.

 This can be extended to any depth.  With embedded containers calling their handlers which
 can be a parent that itself is an embedded container!  Eventually (!) the recursive
 handler calls from Container Manager to handlers, back to Container Manager, back to
 handlers, etc. will "bubble up" to a physical I/O handler.  To keep your sanity, don't
 think about that!  Just think of a physical parent calling a single embedded container for
 a CMValue.

 Since we are simulating "I/O" with these handlers, we need to keep track of the a current
 "stream" position within the value and embedded container (value) size. The position and
 size fields in the struct serves this purpose.

 The other fields (traceFile and tracing) are used for tracing the behavior of this example
 handler. They are manipulated by another special routine setEmbeddedHandlersTrace()
 described later.

 The typeName is also used bor tracing.  But it is useful for error reporting as well. We
 put in it the metahandler type that is passed to CMOpen[New]Container() that causes it to
 use these handlers.  If it's given a unique type for each container, then the error
 messages and the tracing info can be tied back to a specified container.  It is acting as
 the functionaly equivalent of the pathname we used in the non-embedded case.  Note the
 struct only shows where the typeName starts.  Enough space will be allocated to hold the
 entire typeName.

 In this particular example, we define the attributes and the refCon as one in the same.
 They need not be however.  The main reason we do this is to allow tracing of the open
 itself. The refcon/attributes defined here can be passed to a special trace enable/disable
 routine (setEmbeddedHandlersTrace() described later) before we call CMOpen[New]Container()
 to allow tracing of it.  If it were not for the tracing, the attributes would probably be
 just a the CMValue and the refCon would be set up by the open handler.

 Note, the setup we do here follows very closely to that of the non-embedded case.  The
 only real differences is that we do API calls to do the I/O rather than stream I/O calls
 and the refCon is slightly different.  We could have defined a common refCon to be used
 by both embedded and non-embedded cases. Then we could use a single trace setup routine.

 There are a lot of degrees of freedom here how you want to do this.  This is just one
 example.

 The next section is a convenient place to define common ("housekeeping") routines or
 whatever.  Here we define only one, a central display routine to handle tracing output.
 All "printf" trace output goes through the display() routine.

 Note, as mentioned earlier, error displays do NOT go through display().  The session error
 handler is used just as the session memory allocator and deallocator are used to handle
 memory.                                                                                */


/*---------------------------------------*
 | display - isolate all "printf" output |
 *---------------------------------------*

 In some environments it may be more desirable (or necessary) to handle "printf" type
 displays specially. To that end this routine is provided.  All the trace printing goes
 through  here.  It's parameters are identical to fprintf(), with the same meaning.  So,
 as delivered, in its simplest form, this routine maps into a fprintf().  Feel free to
 "warp" this routine into somthing appropriate to your system.
*/

static void CM_NEAR CM_C display(FILE *stream, const char *format, ...)
{
  va_list ap;

  va_start(ap, format);
  vfprintf(stream, format, ap);
  va_end(ap);
}


/* Now for the interesting stuff...                                                      */


/*------------------------------------------------------------------------------*
 | createRefConForMyEmbHandlers - create an embedded container handler "refCon" |
 *------------------------------------------------------------------------------*

 You call this special routine BEFORE calling CMOpen[New]Container().  The thing returned
 from here is the refCon/attributes that will be passed to the open handler in this file.
 In this example, the attributes and the refCon are the same.  So open will return it to
 the Container Manager.  To the Container Manager, the thing returned from the open is a
 "refCon".  That is then passed to all other handler calls.

 Note, callers outside this file as well as the Container Manager have no knowledge of what
 the refCon means. Nor should they!

 This routine allocates the refCon as a MyRefCon struct.  Each MyRefCon struct is variable
 length as a function of the container type name.  This is derived from the passed
 parentValue which is also saved in the struct along with the trace information.  The
 value was created prior to calling this routine and typed as an embedded container type.
 It is saved in the refCon so that the other handler routines can call the Container
 Manager to do "I/O" on it.

 The type name will be used for error reports and tracing.

 Since this is an example, any errors result in termination of the program with an error
 message.
*/

CMRefCon CM_FIXEDARGS createRefConForMyEmbHandlers(const CMValue parentValue)
{
  CMContainer  container   = CMGetValueContainer(parentValue);
  CMSession    sessionData = CMGetSession(container);
  MyRefConPtr p;
  char        typeName[512];

  CMGetContainerInfo(container, NULL, NULL, NULL, typeName, NULL);

  p = (MyRefConPtr)CMMalloc(sizeof(MyRefCon) + strlen(typeName), sessionData);

  if (p == NULL) {                                    /* allocation failed!              */
#ifdef _UNICODE_
    WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                   OD_ALLOCATEFAILEDCONTAINER, sizeof(szMsg1), szMsg1);
    CMError(sessionData, szMsg1, typeName);
#else
    CMError(sessionData, "Allocation of \"refCon\" failed for embedded container (type \"^0\")!", typeName);
#endif
    return (NULL);
  }

  p->sessionData = sessionData;                        /* save the current session ptr    */
  p->parentValue = parentValue;                        /* save value for "I/O" calls      */
  p->traceFile    = NULL;                              /* set only by set...Trace()      */
  p->tracing      = 0;                                  /* ...ditto                        */
  p->position     = 0;                                  /* container not "open" yet        */
  p->size         = 0;                                  /* don't know size yet            */
  strcpy(p->typeName, typeName);                      /* copy in the container typeName  */

  return ((CMRefCon)p);                                /* return refCon as anonymous ptr  */
}


/*----------------------------------------------------------------------*
 | embeddedContainerMetahandler - return the embedded handler addresses |
 *----------------------------------------------------------------------*

 A REQUIRED routine that MUST be called BEFORE calling CMOpen[New]Container() (but after
 CMStartSession()) is CMSetMetaHandler().  You pass to it an address of a metahandler
 procedure to be associated with a particular container type name (string).

 When CMOpen[New]Container() is called, you pass the same type name as one of the
 arguments.  CMOpen[New]Container(), in turn, uses that type name to find the metahandler
 address for the type as defined by the earlier CMSetMetaHandler() call.

 CMOpen[New]Container() now uses the resulting metahandler address and repeatedly calls
 it to get the addresses of all the handler routines it uses.  That is what this routine
 is -- a metahandler that is was set by a CMSetMetaHandler() call.

 For each call a unique operation type string is passed to indicate which routine address
 is to be returned.  The following handlers are needed by the Container Manager.  They
 should have the indicated prototypes:                                                  */

 static CMRefCon open_Handler(CMRefCon attributes, CMOpenMode mode);
 static void close_Handler(CMRefCon refCon);
 static CMSize flush_Handler(CMRefCon refCon);
 static CMSize seek_Handler(CMRefCon refCon, CM_LONG posOff, CMSeekMode mode);
 static CMSize tell_Handler(CMRefCon refCon);
 static CMSize read_Handler(CMRefCon refCon, CMPtr buffer, CMSize elementSize, CMCount theCount);
 static CMSize write_Handler(CMRefCon refCon, CMPtr buffer, CMSize elementSize, CMCount theCount);
 static CMEofStatus eof_Handler(CMRefCon refCon);
 static CMBoolean trunc_Handler(CMRefCon refCon, CMSize containerSize);
 static CMSize containerSize_Handler(CMRefCon refCon);
 static void readLabel_Handler(CMRefCon refCon, CMMagicBytes magicByteSequence,
                               CMContainerFlags *flags, CM_USHORT *bufSize,
                               CM_USHORT *majorVersion, CM_USHORT *minorVersion,
                               CMSize *tocOffset, CMSize *tocSize);
 static void writeLabel_Handler(CMRefCon refCon, CMMagicBytes magicByteSequence,
                                CMContainerFlags flags, CM_USHORT bufSize,
                                CM_USHORT majorVersion, CM_USHORT minorVersion,
                                CMSize tocOffset, CMSize tocSize);
 static CMValue returnParentValue_Handler(CMRefCon refCon);
 static CM_UCHAR *returnContainerName_Handler(CMRefCon refCon);
 static CMType returnTargetType_Handler(CMRefCon refCon, CMContainer container);
 static void extractData_Handler(CMRefCon refCon, CMDataBuffer buffer,
                                  CMSize size, CMPrivateData data);
 static void formatData_Handler(CMRefCon refCon, CMDataBuffer buffer,
                                 CMSize size, CMPrivateData data);
                                                                                         /*
 The documentation for these routines is specified below as each routine is defined so we
 will not repeat it here.  Note, that we actually declare all these routines as "static"
 since they only need to be visible in this file.  The Container Manager gets at them
 through the addresses we return from here.

 NULL may be returned from here.  That indicates to the Container Manager that the routine
 is not provided.  It is an error to not provide a routine required by the Container
 Manager as a function of whether you are opening the container for reading (i.e.,
 CMOpenContainer()) or writing (i.e., CMOpenNewContainer() -- this includes converting).
 Here is a chart as to which routines are required for reading, writing, and updating:

                                        Mode
                           Reading  |  Writing  | Updating
                           ---------+-----------+---------
                open           X     |     X     |    X
                close          X     |     X     |    X
                flush                |           |
                seek          X     |     X     |    X
                tell          X     |     X     |    X
                read           X     |           |    X
                write                |     X     |    X
                eof                  |           |
                trunc                |           |
                size          X     |     X     |    X
                readLabel     X     |           |    X
                writeLabel          |     X     |    X
                parent        1     |     1     |
                name                |           |
                targetType          |           |    X
                extract       X     |     X     |    X
                format        X     |     X     |    X
                           ---------+-----------+---------

 Notes: 1. The parent value handler is required ONLY for embedded container handlers.

 The X's indicate required for the mode.  Blanks mean optional or are not used for the
 mode.  Note, that updating generally is an or'ing of the reading and writing cases.

 Note, these (and the special handlers) are the ONLY routines required by the Container
 Manager itself.  Others may be supplied for special purposes as described in the API
 documentation.  The additional routines we define in this file, namely
 createRefConForMyEmbHandlers() and setEmbeddedHandlersTrace() (described later) are
 supplied just as a mechanism to isolate this example handler and its refCon to this file.
 They are not known to the Container Manager itself.

 So at last we come to the definition of our example metahandler proc. It's a lot shorter
 that the documentation it took to get to this point!  You will see a list of macro calls
 for the operationType strings.  These are defined in     CMAPIIDs.h     which is also
 included by CM_API.h.  Since our example is an illustration for both reading and writing
 all routines are defined in this file.

 The API generalizes the use of metahandlers to allow specific metahandlers for specific
 type objects within a container.  This is the targetType argument.  For simple direct
 handlers, this is not needed and passed as NULL.  It is ignored here.
*/

CMHandlerAddr CM_FIXEDARGS embeddedContainerMetahandler(CMType targetType, CMconst_CMGlobalName operationType)
{
  static char *operationTypes[] = {CMOpenOpType,            /*  0 */ /* Operation Types  */
                                   CMCloseOpType,            /*  1 */
                                   CMFlushOpType,            /*  2 */
                                   CMSeekOpType,            /*  3 */
                                   CMTellOpType,            /*  4 */
                                   CMReadOpType,            /*  5 */
                                   CMWriteOpType,            /*  6 */
                                   CMEofOpType,              /*  7 */
                                   CMTruncOpType,            /*  8 */
                                   CMSizeOpType,            /*  9 */
                                   CMReadLblOpType,          /* 10 */
                                   CMWriteLblOpType,        /* 11 */
                                   CMParentOpType,          /* 12 */
                                   CMContainerOpName,        /* 13 */
                                   CMTargetTypeOpType,      /* 14 */
                                   CMExtractDataOpType,      /* 15 */
                                   CMFormatDataOpType,      /* 16 */
                                   NULL};
  char    **t;
  CMType ignored = targetType;

  /* Look up the operation type in the operationTypes table above...                    */

  t = operationTypes - 1;
  while (*++t) if (strcmp((char *)operationType, *t) == 0) break;

  /* Now that we got it (hopefully), return the appropriate routine address...          */

  switch (t - operationTypes) {
    case  0:  return ((CMHandlerAddr)open_Handler);              /* CMOpenOpType         */
    case  1:  return ((CMHandlerAddr)close_Handler);            /* CMCloseOpType         */
    case  2:  return ((CMHandlerAddr)flush_Handler);            /* CMFlushOpType         */
    case  3:  return ((CMHandlerAddr)seek_Handler);              /* CMSeekOpType         */
    case  4:  return ((CMHandlerAddr)tell_Handler);              /* CMTellOpType         */
    case  5:  return ((CMHandlerAddr)read_Handler);              /* CMReadOpType         */
    case  6:  return ((CMHandlerAddr)write_Handler);            /* CMWriteOpType         */
    case  7:  return ((CMHandlerAddr)eof_Handler);              /* CMEofOpType           */
    case  8:  return ((CMHandlerAddr)trunc_Handler);/* Note 3 *//* CMTruncOpType        */
    case  9:  return ((CMHandlerAddr)containerSize_Handler);    /* CMSizeOpType         */
    case 10:  return ((CMHandlerAddr)readLabel_Handler);        /* CMReadLblOpType       */
    case 11:  return ((CMHandlerAddr)writeLabel_Handler);        /* CMWriteLblOpType     */
    case 12:  return ((CMHandlerAddr)returnParentValue_Handler);/* CMParentOpType        */
    case 13:  return (NULL);                        /* Note 2 *//* CMContainerOpName    */
    case 14:  return (NULL);                        /* Note 4 *//* CMTargetTypeOpType    */
    case 15:  return ((CMHandlerAddr)extractData_Handler);      /* CMExtractDataOpType  */
    case 16:  return ((CMHandlerAddr)formatData_Handler);        /* CMFormatDataOpType    */

    default:  return (NULL);
  }

  /* Notes:                                                                              */

  /*  1. Unlike    XHandlrs.c    , a routine is defined for CMParentOpType.  This is    */
  /*      the only place one is required.  See returnParentValue_Handler for further      */
  /*      details.   IT IS THIS HANDLER THAT DEFINES THIS FILE AS AN EMBEDDED VALUE       */
  /*     HANDLER PACKAGE.                                                                */

  /*   2. Another difference from the    XHandlrs.c     is that NULL is returned for the  */
  /*      name handler.  This will cause the embedded container metahandler type name     */
  /*     passed to CMOpen[New]Container() to be used.  A handler has been provided as a */
  /*     "place-holder" in this file however.  Doing so permitted further documentation */
  /*      about the name handler.  You might want to look at it and possibly make the     */
  /*      changes suggested there.                                                        */

  /*   3. For embedded handlers, we can return a handler address for CMTruncOpType.  See */
  /*     its documentation for further details.                                          */

  /*   4. The returnTargetType_Handler is not supported for embedded handlers because it  */
  /*      is not needed!  It is only needed for primary container I/O handler packages   */
  /*     as described in ExampleHanlders.c, and then only if updating separate target   */
  /*     containers is to be supported.                                                  */
}


/*-----------------------------------------------------------*
 | setEmbeddedHandlersTrace - turn handler tracing on or off |
 *-----------------------------------------------------------*

 "For your viewing pleasure" the example handlers provide a self tracing capability to
 allow you to view the container manipulations.  The trace is written to the specified
 tracingFile (assumed already open).  The tracing may be toggled on and off by passing
 tracingState as 1 (on, actually any non-zero value) or 0 (off) with repeated calls to
 this routine.

 The trace assumes that the tracingFile is a text file.  Only text file writes, i.e.,
 printf()'s are used to write to the file.  It is up to the caller to open and close the
 file if necessary.

 Remember this routine is NOT part of a standard handler.  It just facilitates the use of
 this particular example.

 Note, for convenience (?) this routines returns the refCon as its result.
*/

CMRefCon CM_FIXEDARGS setEmbeddedHandlersTrace(CMRefCon refCon, const char tracingState,
                                                FILE CM_PTR *tracingFile)
{
  ((MyRefConPtr)refCon)->traceFile = tracingFile;
  ((MyRefConPtr)refCon)->tracing    = (char)(tracingState && tracingFile);

  return (refCon);                                          /* return our refCon         */
}


/*--------------------------------------------------------------------------------------*
  From this point one we have the handler routines themselves.  Order doesn't matter from
  here on.  Read them at your leisure (yea, right!)

  Note: To keep the example simple, there is little or no error checking.  More error
        checking should be provided where applicable.
 *--------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------*
 | open_Handler - open a container for input, output, or updating |
 *----------------------------------------------------------------*

 This is called only by CMOpen[New]Container() to open the embedded container associated
 with the specified attributes.  The open modes passed are "wb+" (truncate and updating)
 for creating a new embedded container, "rb" for reading an existing embedded container,
 and "rb+" (updating) for converting a "bunch" of data in a file to container format.

 The open routine is unique among the handlers in that the potential interpretion of the
 first argument is different.  The attributes are passed to CMOpen[New]Container() and
 then passed to here unaltered and not looked at by the Container Manager.  It is intended
 to tie the open handler to a specific container.  Thus the attributes serves as a
 communication channel strictly to the open. In its simplest form for an embedded
 container, it probably would be the parent value (CMValue), corresponding to the embedded
 container.

 What the Container Manager expects as a return value is a "refCon" to be passed unaltered
 to all of the other handler routines.  This too is a communication channel that ties the
 handlers to their open.  Again, in its simplest form the CMValue attributes would probably be
 viewed as the refCon.  It would be returned to the Container Manager which would pass it
 to all following handler calls.  These would then do API "I/O" calls using the CMValue.

 In our example, we defined the attributes and the refCon to be the same thing.  That is a
 pointer to a struct that contains the CMValue and some trace info. This is allocated by a
 previous call to createRefConForMyEmbHandlers() which set the CMValue.  Thus the input
 attributes is simply returned to the Container Manager.  Thus all the other handlers will
 know that the refCon is a pointer to our struct.

 The modes of container (file) opening passed here from the Container Manager are the same
 as for non-embedded containers.  There are three open modes to go with the three open cases
 supported by the Container Manager:

 converting   For this mode the open mode is "rb+".  The intent is to open an embedded
               container for updating, i.e., reading and writing, but preserve the current
              contents of the file. [It's not really clear what this means for embedded
              containers!]

 writing      The mode here is "wb+".  The intent is to create the embedded container. It
               is an error if there already value data defined.  The Container Manager
              allows reading of stuff previously written.

 reading      The read open mode is "rb" (no plus). An existing embedded container is to be
               open for input (reading) only.  It cannot be written to for updating.  It is
              an error if there is no value data defined for this value.

 updating      The read open mode is "rb+".  An existing container is to be opened for
              updating.  This is also used for reusing free space.

 Unlike the stream open defined in    XHandlrs.c    , more work must be done here doing
 error checks.  The reason for requiring a specific type (a predefined standard type object
 descriptor) is for safety reasons. This prevents sending arbitrary values to the handlers.

 For writing there must not be any value data defined for the value.  The use of such a
 value for creating an embedded container takes the place of CMWriteValueData(). Similarly
 embedded containers are read using handlers instead of CMReadValueData().  There is
 nothing preventing using CMReadValueData() on a value defined as an embedded container.
 This allows reading of the entire embedded container for copying purposes.

 Note, that although we must do error checks here no other "real" work needs to be done.
 There is no formal stream open to go through.  So once we're happy with the value we
 return the attributes as the refCon.

 This is a REQUIRED handler routine.
*/

static CMRefCon open_Handler(CMRefCon attributes, CMOpenMode mode)
{
  MyRefConPtr p = (MyRefConPtr)attributes;

  /* Get the size of the value.  For writing it better be 0 and for reading non-zero...  */

  p->size = (unsigned long)CMGetValueSize(p->parentValue);

  /* Check the open mode and do appropriate checks on size. Set appropriate position...  */

  if (strcmp((char *)mode, "wb+") == 0) {        /* writing...                             */
    if (p->size != 0) {                          /* size must be zero                    */
#ifdef _UNICODE_
      WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                     OD_CANNOTCREATECONTAINER, sizeof(szMsg1), szMsg1);
      CMError(p->sessionData, szMsg1, p->typeName);
#else
      CMError(p->sessionData, "Cannot create embedded container (type \"^0\") for a value that already has data!", p->typeName);
#endif
      return (NULL);
    }
    p->position = 0;                            /* position to 1st free byte            */
  } else if (strcmp((char *)mode, "rb") == 0) { /* reading...                             */
    if (p->size == 0) {                          /* size must be non-zero                */
#ifdef _UNICODE_
      WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                     OD_CANNOTREADCONTAINER, sizeof(szMsg1), szMsg1);
      CMError(p->sessionData, szMsg1, p->typeName);
#else
      CMError(p->sessionData, "Cannot read embedded container (type \"^0\") for a value that doesn't have data!", p->typeName);
#endif
      return (NULL);
    }
    p->position = 0;                            /* position to 1st byte to read          */
  } else if (strcmp((char *)mode, "rb+") == 0)  /* converting or updating...            */
    p->position = p->size;                      /* size can be anything(position at end)*/
  else {                                        /* bad mode...                          */
#ifdef _UNICODE_
    WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                   OD_INVALCONTAINER, sizeof(szMsg1), szMsg1);
    CMError(p->sessionData, szMsg1, p->typeName, (char *)mode);
#else
    CMError(p->sessionData, "Invalid embedded container (type \"^0\") open mode (\"^1\")!", p->typeName, (char *)mode);
#endif
    return (NULL);
  }

  if (p->tracing)                                        /* tracing...                    */
    display(p->traceFile, "open_Handler(<%s>, \"%s\")\n", p->typeName, (char *)mode);

  return (attributes);                          /* attributes is same as refCon here    */
}


/*----------------------------------------------------------------------------------*
 | returnParentValue_Handler - return value refNum for parent of embedded container |
 *----------------------------------------------------------------------------------*

 This handler routine is used ONLY for embedded containers.  It is called at open time by
 CMOpen[New]Container() so that the Container Manager may determine for itself that it is
 opening an embedded container for a value and what that value is. It is the parent CMValue
 for this handler that is retured by this function.

 The parameters to CMOpen[New]Container() do NOT indicate that an embedded container is
 being open. The only thing that determines that is the fact that a metahandler type
 defines a set of handlers that know how to deal with an embedded container. In other
 words, the handlers in this file.

 For non-embedded containers, the metahandler would return NULL for this handler address.
 NULL indicates there is no handler for an operation.  So the Container Manager would know
 that it is not opening an embedded container.  Alternatively, NULL can be returned from
 here to indicate the same thing.

 The Container Manager needs to know it is dealing with an embedded container value mainly
 so that it can keep track of all open descendent embedded containers of a parent (possibly
 itself embedded) container.  This allows the Container Manager to keep track of the
 tree (!) of open containers.  By knowing the tree, CMCloseContainer() can be used to close
 a parent and ALL of its descendents with a single call!

 This is a REQUIRED handler routine for embedded containers only.  Indeed it is THE ROUTINE
 that defines this package to the Container Manager as an embedded container handler
 package!
*/

static CMValue returnParentValue_Handler(CMRefCon refCon)
{
  MyRefConPtr p = (MyRefConPtr)refCon;

  if (p->tracing)                                        /* tracing...                    */
    display(p->traceFile, "returnParentValue_Handler(<%s>)\n", p->typeName);

  return (p->parentValue);
}


/*------------------------------------------*
 | close_Handler - close the container file |
 *------------------------------------------*

 This is called when CMCloseContainer() is called.  The space allocated by
 createRefConForMyEmbHandlers() for our refCon is freed.  Once the Container Manager
 calls this routine, it will, of course, make no further calls the the handler.

 For embedded containers, there is no formal close as in the non-embedded case illustrated
 in ExampleHandlers.c.  All we have to do is free the refCon space.

 This is a REQUIRED handler routine.
*/

static void close_Handler(CMRefCon refCon)
{
  MyRefConPtr p = (MyRefConPtr)refCon;
  CMSession    sessionData = p->sessionData;

  if (p->tracing)                                        /* tracing...                    */
    display(p->traceFile, "close_Handler(<%s>)\n"
                          "/////////////////////////////////////////"
                          "//////////////////////////////////////////////\n", p->typeName);

  CMFree(p, sessionData);                                /* bye, bye, refcon...          */
}


/*---------------------------------------*
 | flush_Handler - flush the I/O buffers |
 *---------------------------------------*

 This routine is called by the Container Manager when it wants to make sure the I/O
 (actually just output) buffer(s) are written to the container.  In most systems it is
 probably unnecessary.  For embedded containers it's definitely is!

 0 is returned for success and non-zero for failure.

 This is an OPTIONAL routine which will be called by the Container Manager when it thinks
 it needs it. If it is not defined, it is ignored on the assumtion the handler writer knows
 it is unnecessary.
 */

static CMSize flush_Handler(CMRefCon refCon)
{
  MyRefConPtr p = (MyRefConPtr)refCon;

  if (p->tracing)                                        /* tracing...                    */
    display(p->traceFile, "flush_Handler(<%s>)\n", p->typeName);

  return ((CMSize)0);
}


/*-------------------------------------------------------------------*
 | seek_Handler - position the I/O to the specified container offset |
 *-------------------------------------------------------------------*

 This allows the Container Manager random access to the embedded container. The current
 position value is set according to the mode and posOff.  0 is returned for success and
 non-zero for failure. For embedded containers there is no failure (famous last words).

 The mode determines how to use posOff to set the position. It is defined as one of the
 following seek modes (defined in   CMAPITyp.h   which is included by CMAPI.h ):

  kCMSeekSet          Set the position to the value offset specified by posOff.  The posOff
                      is the character offset from the beginning of the value data.

  kCMSeekCurrent      Set the stream posOff characters from the current position.  posOff
                      can be postive or negative here.

  kCMSeekEnd          Set the position posOff characters from the current end of the value.
                      posOff here is only zero or negative.  We never assume or allow
                      undefined contents to be generated in a container. This makes life
                      easier here since we don't have to check for this.

 Note, here all we do is set our position saved in the refCon.  There is nothing else to
 do.  The position is used by other handlers such as the reads and writes.

 This is a REQUIRED handler routine.
*/

static CMSize seek_Handler(CMRefCon refCon, CM_LONG posOff, CMSeekMode mode)
{
  MyRefConPtr p = (MyRefConPtr)refCon;
  CMSize      result = 0;

  if (mode == kCMSeekSet)                          /* kCMSeekSet...                      */
    p->position = (unsigned long)posOff;          /* ...just use posOff as is            */
  else if (mode == kCMSeekEnd)                    /* kCMSeekEnd...                      */
    p->position = (long)p->size + posOff;          /* ...posOff is 0 or negative here    */
  else                                             /* kCMSeekCurrent...                   */
    p->position = (long)p->position + posOff;      /* ...relative positioning            */

  if (p->tracing)                                  /* tracing...                          */
    display(p->traceFile, "seek_Handler(<%s>, 0x%.8lX = %ld, %s) --> %ld, position = %ld = 0x%.8lX\n",
            p->typeName, posOff, posOff,
            (mode == kCMSeekSet) ? "kCMSeekSet"
                                 : ((mode == kCMSeekEnd) ? "kCMSeekEnd"
                                                          : "kCMSeekCurrent"), result,
            p->position, p->position);

  return (result);
}


/*------------------------------------------------------------------------*
 | tell_Handler - return current position in the embedded container value |
 *------------------------------------------------------------------------*

 This routine returns the current position as characters from the start of the embedded
 container value.

 The position is set by other handlers like the seek (above) and the reads and writes.

 This is a REQUIRED handler routine.
*/

static CMSize tell_Handler(CMRefCon refCon)
{
  MyRefConPtr p = (MyRefConPtr)refCon;

  if (p->tracing)                                        /* tracing...                    */
    display(p->traceFile, "tell_Handler(<%s>) --> 0x%.8lX = %ld\n", p->typeName, p->position, p->position);

  return ((CMSize)p->position);
}


/*---------------------------------------------------------*
 | displayIOBuffer - tracing routine to display I/O buffer |
 *---------------------------------------------------------*

 This is NOT a handler routine.  It is called by the read_Handler() and write_Handler()
 handlers only when tracing is on to trace the stuff being read or written.

 The display is formatted as 16 characters per line grouped in 4-byte chunks.  The
 characters for the hex bytes is shown to the right.  Each line shows the container offset
 for that line.  Here is a example of the display:

   12345678: XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX  cccccccccccccccc

 The offset is shown in hex, but the "0x" prefix is not displayed.

 All the args here are the same as the handlers except for the additional starting offset
 value, fileOffset. For writes we get the offset before the write to pass to here.  But
 for reads we need the offset before we do the read to be able to call here after the
 read to get the display correct.
*/

static void CM_NEAR displayIOBuffer(CMRefCon refCon, unsigned long fileOffset, CMPtr buffer,
                                    CMSize elementSize, CMCount theCount)
{
  MyRefConPtr   p = (MyRefConPtr)refCon;
  unsigned char charBufr[17], *s, *b = (unsigned char *)buffer;
  long          i, n = theCount * elementSize;

  /* Extract 16 bytes at a time from the buffer...                                      */

  while (n > 0) {                                    /* loop across the entire buffer...  */
    display(p->traceFile, "   %.8lX: ", fileOffset);/*     output offset for this line    */
    i = 16;                                          /*     show 16-bytes per line        */
    s = charBufr;                                    /*     put characters in here        */
    while (n > 0 && i-- > 0) {                      /*     generate up to the 16 bytes...*/
      *s++ = (unsigned char)((*b < (unsigned char)' ' ||  *b > (unsigned char)126) ? (unsigned char)'.' : *b);
      display(p->traceFile, "%.2X", *b++);          /*        output a hex byte          */
      if ((i & 3) == 0) display(p->traceFile, " ");  /*        put into groups of 4      */
      ++fileOffset;                                  /*        set for next byte          */
      --n;
    }

    while (i-- > 0) {                                /*    pad out short hex lines...    */
      if ((i & 3) == 0) display(p->traceFile, " ");  /*        be carfull to account for */
      display(p->traceFile, "  ");                  /*        the 4-byte groups          */
    }

    *s = 0;                                          /*     display chars for the hex...  */
    display(p->traceFile, "  %s\n", charBufr);
  } /* while */                                      /* loop for next group of 16        */
}


/*------------------------------------------------------------------*
 | read_Handler - read information from the container into a buffer |
 *------------------------------------------------------------------*

 This handler reads bytes from the current position with the embedded container value into
 the specified buffer.  Up to theCount elements of the specified size are read. The actual
 number of items read, which may be less than theCount (e.g., if the end-of-container is
 read) or even 0 is returned as the function result.

 Note, if you are reading these routines sequentially, this is the first routine that
 formally does calls back to the API to do the "I/O".

 This is a REQUIRED ROUTINE FOR READING only (CMOpenContainer()).
*/

static CMSize read_Handler(CMRefCon refCon, CMPtr buffer, CMSize elementSize, CMCount theCount)
{
  MyRefConPtr    p = (MyRefConPtr)refCon;
  unsigned long fileOffset, amountRead;

  if (p->tracing)  fileOffset = p->position;      /* tracing (need initial offset)        */

  amountRead = (unsigned long)CMReadValueData(p->parentValue,   /* read value...        */
                                               buffer,
                                               p->position,
                                               (CMSize)((unsigned long)elementSize * (unsigned long)theCount));

  p->position += amountRead;                    /* update position by amount read        */
  amountRead  /= (unsigned long)elementSize;    /* convert back to elementSize units    */

  if (p->tracing)  {                              /* tracing...                            */
    display(p->traceFile, "read_Handler(<%s>, %ld, %ld) --> %ld\n", p->typeName,
                          elementSize, theCount, amountRead);
    if (amountRead) displayIOBuffer(refCon, fileOffset, buffer, elementSize, theCount);
  }

  return ((CMSize)amountRead);
}


/*------------------------------------------------------------------*
 | write_Handler - write information from a buffer to the container |
 *------------------------------------------------------------------*

 This handler writes bytes from the specified buffer to the embedded container value at
 the current value position.  Up to theCount elements of the specified size are written.
 The actual number  of items written is returned.  It should be the same as theCount unless
 errors occur.  The Container Manager checks this so it need not be checked here.

 Note, this is the only place that sets the size field in our refCon struct.  In all other
 places it is a constant.  For reading a existing value it is always the fixed size of
 the value.  Only for writing can it change, and then only if we write past the current
 "eof" for the value.

 This is a REQUIRED ROUTINE FOR WRITING only (CMOpenNewContainer()).
*/

static CMSize write_Handler(CMRefCon refCon, CMPtr buffer, CMSize elementSize,
                            CMCount theCount)
{
  MyRefConPtr   p = (MyRefConPtr)refCon;
  unsigned long amountWritten = (unsigned long)elementSize * (unsigned long)theCount;

  if (p->tracing)  {                                /* tracing (do 1st!)...                */
    display(p->traceFile, "write_Handler(<%s>, %ld, %ld) --> %ld\n", p->typeName,
                          elementSize, theCount, amountWritten);
    if (amountWritten) displayIOBuffer(refCon, p->position, buffer, elementSize, theCount);
  }

  CMWriteValueData(p->parentValue, buffer, p->position, (CMSize)amountWritten);

  p->position += amountWritten;                    /* update position by amount read      */

  if (p->position > p->size)                      /* if writing past end of value...    */
    p->size = p->position;                        /* ...set new value size              */

  return ((CMSize)theCount);
}


/*------------------------------------------*
 | eof_Handler - determine eof input status |
 *------------------------------------------*

 This handler checks to see if an end-of-file was detected while reading the container (by
 read_Handler()).  If it has a non-zero value is returned.  Otherwise, 0 is returned.

 This is an OPTIONAL routine.  It is not used by the Container Manager itself, but might be
 used by handlers for special types.  Certainly for embedded containers, there is nothing
 to do!
*/

static CMEofStatus eof_Handler(CMRefCon refCon)
{
  MyRefConPtr p = (MyRefConPtr)refCon;

  if (p->tracing)                                        /* tracing...                    */
    display(p->traceFile, "eof_Handler(<%s>) --> %ld\n", p->typeName, 0L);

  return ((CMEofStatus)0);
}


/*---------------------------------------------------------*
 | trunc_Handler - truncate container to the specifed size |
 *---------------------------------------------------------*

 This handler is expected to truncate the container to the specified size.  1 is returned
 if it is truncated and 0 otherwise.

 This is an OPTIONAL routine used only for reusing free space where it is possible for a
 new updated TOC is smaller than the original.  The excess space must be removed or
 declared as free space.  This routine, if it is provided, should do the removal.  If not
 supplied, or if it returns false, then potentially it will take TWO writes of the TOC to
 move the excess space on to the container's free list and to its proper place before the
 TOC.

 Unlike the suitation in    XHandlrs.c    , here we can define a truncation routine by
 doing a CMDeleteValueData() on the parent value.  In    XHandlrs.c     we were using
 standard ANSI stream I/O which provides no way of truncation a stream.  Here we don't
 have a stream.
*/

static CMBoolean trunc_Handler(CMRefCon refCon, CMSize containerSize)
{
  MyRefConPtr p = (MyRefConPtr)refCon;

  if (p->tracing)                                        /* tracing...                    */
    display(p->traceFile, "trunc_Handler(\"%s\") --> %ld = 0x.8lX\n",
                          p->typeName, containerSize, containerSize);

  CMDeleteValueData(p->parentValue,                     /* delete excess stuff          */
                    (CMCount)containerSize,
                    (CMSize)(p->size - (unsigned long)containerSize));

  p->size  = (unsigned long)containerSize;                /* set new size                  */

  return (1);
}


/*------------------------------------------------------------------*
 | containerSize_Handler - return the current size of the container |
 *------------------------------------------------------------------*

 This routine is used to get the size of the container. It returns the 1-relative size of
 the value (i.e., embedded container).

 To make this as equivalent as possible to the stream version of this handler, we do the
 "moral equivalent" of a fseek to the end of the container followed by a ftell.  For
 embedded containers this is simply the result of setting the current position pointer to
 be the size of the value.
*/

static CMSize containerSize_Handler(CMRefCon refCon)
{
  MyRefConPtr p = (MyRefConPtr)refCon;

  p->position = p->size;                                /* moral equivalent of seek/tell*/

  if (p->tracing)                                        /* tracing...                    */
    display(p->traceFile, "containerSize_Handler(\"%s\") --> %ld\n", p->typeName, p->size);

  return ((CMSize)p->size);
}


/*---------------------------------------------------------------*
 | readLabel_Handler - read in and extract the container's label |
 *---------------------------------------------------------------*

 This handler finds the label in the embedded container value and returns all the
 information it contains. In order to make embedded container values have the same format
 as the stream non-embedded containers which the    XHandlrs.c     handlers maintain, we
 will put and look for the label at the end of the embedded container value.

 As defined by the API documentation, a container label has the following layout:

      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
      |      Magic Bytes      |flags|bufsz|major|minor|TOC offset |  TOC size |
      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
                  8              2     2     2     2        4           4

 For C compilers that generate code for machines where char is 1 byte, short 2, and long
 4, we may use the following struct to define the label format:                          */

 struct ContainerLabelFmt {                /* Layout of a container label:                */
   unsigned char  magicBytes[8];          /* 8 bytes: the magic byte identifier          */
   unsigned short flags;                  /* 2        the label flags                    */
   unsigned short bufSize;                /* 2        TOC buffer size / 1024            */
   unsigned short majorVersion;            /* 2        major format version number        */
   unsigned short minorVersion;            /* 2        minor format version number        */
   unsigned long  tocOffset;              /* 4        offset to start of TOC            */
   unsigned long  tocSize;                /* 4        total byte size of the TOC        */
 };
 typedef struct ContainerLabelFmt ContainerLabelFmt, *ContainerLabelFmtPtr;              /*

                                    W A R N I N G !!

 If your C compiler and/or hardware cannot use the above struct, then this handler will
 have to be recoded accordingly.  Indeed, it is for this very reason this IS a handler. It
 shifts the portability problem outside the Container Manager.

 This handler looks for the label and decodes it according to the above struct.  So this
 is only for implementations with the above C size definitions.  Our own handler routines
 defined for containers are used to do the positioning and read.

 No editoralizing is done on the extracted information.  It is simply passed back to the
 Container Manager where the validation is done.

 This is a REQUIRED ROUTINE FOR READING only (CMOpenContainer()).
*/

static void readLabel_Handler(CMRefCon refCon, CMMagicBytes magicByteSequence,
                               CMContainerFlags *flags, CM_USHORT *bufSize,
                               CM_USHORT *majorVersion, CM_USHORT *minorVersion,
                                CMSize *tocOffset, CMSize *tocSize)
{
  MyRefConPtr       p = (MyRefConPtr)refCon;
  unsigned long      labelSize;
  ContainerLabelFmt theLabel;

  /* Seek to the end of the label at the end of the value and read it...                */

  seek_Handler(refCon, -(long)sizeof(ContainerLabelFmt), kCMSeekEnd);
  labelSize = (unsigned long)read_Handler(refCon, (CMPtr)&theLabel,
                                          (CMSize)sizeof(unsigned char),
                                          (CMCount)sizeof(ContainerLabelFmt));

  if (labelSize != sizeof(ContainerLabelFmt)) {    /* must have read it all!              */
#ifdef _UNICODE_
    WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                   OD_CANNOTREADLABEL, sizeof(szMsg1), szMsg1);
    CMError(p->sessionData, szMsg1, p->typeName);
#else
    CMError(p->sessionData, "Embedded container (type \"^0\") label could not be read!", p->typeName);
#endif
    return;
  }

  /* Return all the label info...                                                        */

  memcpy(magicByteSequence, theLabel.magicBytes, 8);
  *flags        = (CMContainerFlags)theLabel.flags;
  *bufSize      = (CM_USHORT)theLabel.bufSize;
  *majorVersion = (CM_USHORT)theLabel.majorVersion;
  *minorVersion = (CM_USHORT)theLabel.minorVersion;
  *tocOffset    = (CMSize)theLabel.tocOffset;
  *tocSize      = (CMSize)theLabel.tocSize;

  if (p->tracing)                                  /* tracing...                          */
    display(p->traceFile, "readLabel_Handler(<%s>)\n"
                          "   -------- | %s | %.4X | %.4X | %.4X | %.4X | %.8lX | %.8lX |\n",
                          p->typeName,
                          magicByteSequence, *flags, *bufSize, *majorVersion, *minorVersion,
                          *tocOffset, *tocSize);
}


/*-----------------------------------------------------------*
 | writeLabel_Handler - format and write a container's label |
 *-----------------------------------------------------------*

 This handler is called to write a label to the embedded container value. It is called by
 the Container Manager as the LAST thing prior to closing the embedded container which was
 opened for writing.  In order to make embedded container values have the same format as
 the stream non-embedded containers which the    XHandlrs.c     handlers maintain, we will
 put and look for the label at the end of the embedded container value.  Since everything
 else has already been written, there isn't really any other place we could put it!

 The label format is defined in readLabel_Handler() above. Here we take all the parameters,
 format them according to the label layout, and writes the data to the value.   Our own
 handler routines defined for embedded containers are used to do the positioning and write.

 Note, see discussion in readLabel_Handler() for portability issues. The problems described
 there equally apply here.

 In this example implementation (with 2-byte words and 4-byte longs), the ContainerLabelFmt
 exactly maps over the desired label format.  Hence no special work needs to be done here.

 This is a REQUIRED ROUTINE FOR WRITING only (CMOpenNewContainer()).
*/

static void writeLabel_Handler(CMRefCon refCon, CMMagicBytes magicByteSequence,
                                CMContainerFlags flags, CM_USHORT bufSize,
                                CM_USHORT majorVersion, CM_USHORT minorVersion,
                                CMSize tocOffset, CMSize tocSize)
{
  MyRefConPtr       p = (MyRefConPtr)refCon;
  unsigned long      labelSize;
  ContainerLabelFmt theLabel;

  if (p->tracing)                                /* tracing (do 1st)...                  */
    display(p->traceFile, "writeLabel_Handler(<%s>)\n"
                          "   -------- | %s | %.4X | %.4X | %.4X | %.4X | %.8lX | %.8lX |\n",
                          p->typeName,
                          magicByteSequence, flags, bufSize, majorVersion, minorVersion,
                          tocOffset, tocSize);

  /* Fill in the label buffer with the info...                                          */

  theLabel.flags        = (unsigned short)flags;
  theLabel.bufSize       = (unsigned short)bufSize;
  theLabel.majorVersion = (unsigned short)majorVersion;
  theLabel.minorVersion = (unsigned short)minorVersion;
  theLabel.tocOffset     = (unsigned long)tocOffset;
  theLabel.tocSize      = (unsigned long)tocSize;
  memcpy(theLabel.magicBytes, magicByteSequence, 8);

  /* Write the label to the end of the embedded container value...                      */

  seek_Handler(refCon, 0, kCMSeekEnd);
  labelSize = (unsigned long)write_Handler(refCon, (CMPtr)&theLabel,
                                           (CMSize)sizeof(unsigned char),
                                           (CMCount)sizeof(ContainerLabelFmt));

  if (labelSize != sizeof(ContainerLabelFmt)) {   /* must have written it all!            */
#ifdef _UNICODE_
    WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                   OD_CANNOTWRITELABEL, sizeof(szMsg1), szMsg1);
    CMError(p->sessionData, szMsg1, p->typeName);
#else
    CMError(p->sessionData, "Embedded container (type \"^0\") label could not be written!", p->typeName);
#endif
  }
}


/*---------------------------------------------------------------------------*
 | returnContainerName_Handler - return string that identifies the container |
 *---------------------------------------------------------------------------*

 When the Container Manager reports errors (through the error handler -- see
 ExampleSessionHandlers.[ch]) it passes appropriate string inserts that may be formatted
 into the error message.  One of those inserts is usually an string that identifies for
 which container the error applies.  The handler defined here is used by the Container
 Manager to get that identification.

 Unlike the stream non-embedded container case, we really don't have anything other than
 the type name passed to CMOpen[New]Container() to use. We added it to the refCon info when
 the private routine createRefConForMyEmbHandlers() was called.  So we might as well use
 that. That being the case we can be more subtle! As mentioned below, if we don't define
 this handler to the Container Manager it is the type name that will be used by default.
 So, for this example, we don't (if you don't believe me, take a look at
 embeddedContainerMetahandler()).  The only reason this routine is explicit here is so you
 can read these comments (and to satisfy the forward reference we "slipped" into the
 embeddedContainerMetahandler() comments when you weren't looking).

 Note, that if the definition of createRefConForMyEmbHandlers() were changed (remember,
 it's a private routine to with this specific example handler -- its not a handler routine
 required by the Container Manager), we could pass it some unique identification which we
 could send back from here.  But being lazy by the time I got here...

 This is a OPTIONAL routine for reading and writing.  If not provided, the type name passed
 to CMOpen[New]Container(), i.e., the metahandler type, is used for the identification.
*/

static CM_UCHAR *returnContainerName_Handler(CMRefCon refCon)
{
  return ((CM_UCHAR *)((MyRefConPtr)refCon)->typeName);/*what I would do if I wanted to */
}                                                       /*explicitly return the type name*/


/*------------------------------------------------------------------------------*
 | returnTargetType_Handler - return target type for updating another container |
 *------------------------------------------------------------------------------*

 This routine is only needed by CMOpenNewContainer() for updating a separate container.
 That can never be the case with embedded values.  Hence this routine is not needed.
 See    XHandlrs.c     for complete documentation on the use of this routine for updating.
*/

static CMType returnTargetType_Handler(CMRefCon refCon, CMContainer container)
{
  MyRefConPtr p = (MyRefConPtr)refCon;
  CMContainer unused = container;

  if (p->tracing)                                        /* tracing...                    */
    display(p->traceFile, "returnTargetType_Handler(<%s>)\n", p->typeName);

  return (NULL);
}


/*----------------------------------------------------------------------------*
 | extractData_Handler - extract private Container Manager data from a buffer |
 *----------------------------------------------------------------------------*

 This handler is used to extract "internal" Container Manager data previously written to
 the container (e.g., CMReferences).  1, 2, or 4 bytes (size 8-bit byte) "chunks" of data
 are expected to be copied from a buffer to the data.  Pointers to the data and the buffer
 are passed in.  The buffer can always be assumed large enough to supply all the requested
 data.  The pointer to the data can be assumed to point to a CM_UCHAR if size is 1,
 CM_USHORT if size is 2, and CM_ULONG is size if 4.

 The 1, 2, or 4 bytes are, of course, formatted within the CM_UCHAR, CM_USHORT, or CM_ULONG
 as a function of the architecture.  These may be a different size than what is expected
 to be written to the container.  Indeed, it is the potential difference between the
 architecture from the data layout in the  container that this handler must be provided.

 The information is stored in the container in a layout PRIVATE to the Container Manager.
 For example, it is used to extract the fields of the TOC.  It does repeated calls to this
 handler to extract the information it needs from a buffer that it loads using the
 read_Handler.

 In this example CM_UCHAR, CM_USHORT and CM_ULONG map directly into the container format
 1, 2, and 4 byte entities.  Hence extracting the data is straight-forward.

 This is a REQUIRED handler routine.
*/

static void extractData_Handler(CMRefCon refCon, CMDataBuffer buffer,
                                CMSize size, CMPrivateData data)
{
  MyRefConPtr p = (MyRefConPtr)refCon;
  CM_UCHAR    trace1;
  CM_USHORT    trace2;
  CM_ULONG    trace4;

  if ((long)size < 0) {  /* [120604] this means it is endian-ness netural  */
    size = -(long)size;
  }

  memcpy(data, buffer, (size_t)size);                  /* copy the buffer to the data    */

  if (p->tracing)  {                                    /* tracing...                      */
    if (size == 1) {
      memcpy(&trace1, data, sizeof(CM_UCHAR));
      trace4 = trace1;
    } else if (size == 2) {
      memcpy(&trace2, data, sizeof(CM_USHORT));
      trace4 = trace2;
    } else
      memcpy(&trace4, data, sizeof(CM_ULONG));
    display(p->traceFile, "extractData_Handler(<%s>, %ld, %ld)\n", p->typeName, size, trace4);
  }
}


/*--------------------------------------------------------------------------*
 | formatData_Handler - format private Container Manager data into a buffer |
 *--------------------------------------------------------------------------*

 This handler is used to format "internal" Container Manager data to be written to
 the container (e.g., CMReferences).  1, 2, or 4 bytes (size 8-bit byte) "chunks" of data
 are expected to be copied from the data to a buffer.  Pointers to the data and the buffer
 are passed in.  The buffer can always be assumed large enough to hold the data.  The
 pointer to the data can be assumed to point to a CM_UCHAR if size is 1, CM_USHORT if size
 is 2, and CM_ULONG is size if 4.

 The 1, 2, or 4 bytes are, of course, stored in the CM_UCHAR, CM_USHORT, or CM_ULONG as a
 function of the architecture.  These may be a different size than what is expected to be
 written to the container.  Indeed, it is the potential difference between the architecture
 from the data layout in the container that this handler must be provided.

 The information is stored in the container in a layout PRIVATE to the Container Manager.
 For example, it is used to format the fields of the TOC.  It does repeated calls to this
 handler to format the information it needs into a buffer that is eventually written using
 the write_Handler.

 In this example CM_UCHAR, CM_USHORT and CM_ULONG directly map into the container format
 1, 2, and 4 byte entities.  Hence the formatting is straight-forward.

 This is a REQUIRED handler routine.
*/

static void formatData_Handler(CMRefCon refCon, CMDataBuffer buffer,
                               CMSize size, CMPrivateData data)
{
  MyRefConPtr p = (MyRefConPtr)refCon;
  CM_UCHAR    trace1;
  CM_USHORT    trace2;
  CM_ULONG    trace4;

  if ((long)size < 0) {  /* [120604] this means it is endian-ness netural  */
    size = -(long)size;
  }

  if (p->tracing)  {                                    /* tracing...                      */
    if (size == 1) {
      memcpy(&trace1, data, sizeof(CM_UCHAR));
      trace4 = trace1;
    } else if (size == 2) {
      memcpy(&trace2, data, sizeof(CM_USHORT));
      trace4 = trace2;
    } else
      memcpy(&trace4, data, sizeof(CM_ULONG));
    display(p->traceFile, "formatData_Handler(<%s>, %ld, %ld)\n", p->typeName, size, trace4);
  }

  memcpy(buffer, data, (size_t)size);                  /* copy the data top the buffer    */
}

                              CM_END_CFUNCTIONS
