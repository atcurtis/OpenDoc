/* @(#)Z 1.5 os2/src/storage/XDragDrp.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:51:41 (96/08/23 01:42:55) */
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

/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using: 
 *      SOM Emitter emitxtm: 2.43
 */

#ifndef SOM_Module_dragdrp_Source
#define SOM_Module_dragdrp_Source
#endif
#define ODDragAndDrop_Class_Source

#include "DragDrp.xih"


SOM_Scope void  SOMLINK ODDragAndDropClear(ODDragAndDrop *somSelf, 
                                            Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropClear");

}

SOM_Scope ODStorageUnit*  SOMLINK ODDragAndDropGetContentStorageUnit(ODDragAndDrop *somSelf, 
                                                                      Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetContentStorageUnit");

        ODStorageUnit* retval; return retval;
}

SOM_Scope ODDropResult  SOMLINK ODDragAndDropStartDrag(ODDragAndDrop *somSelf, 
                                                        Environment *ev, 
                                                       ODFrame* srcFrame, 
                                                       ODType imageType, 
                                                       ODByteArray* image, 
                                                       ODPart** destPart, 
                                                       ODByteArray* refCon)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropStartDrag");

        ODDropResult retval; return retval;
}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropShowPasteAsDialog(ODDragAndDrop *somSelf, 
                                                             Environment *ev, 
                                                            ODBoolean canPasteLink, 
                                                            ODPasteAsMergeSetting mergeSetting, 
                                                            ODFacet* facet, 
                                                            ODTypeToken viewType, 
                                                            ODStorageUnit* contentSU, 
                                                            ODPasteAsResult* theResult)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropShowPasteAsDialog");
    return kODFalse;

}

SOM_Scope ODULong  SOMLINK ODDragAndDropGetDragAttributes(ODDragAndDrop *somSelf, 
                                                           Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDragAttributes");

        return 0;
}

SOM_Scope ODPlatformDragReference  SOMLINK ODDragAndDropGetDragReference(ODDragAndDrop *somSelf, 
                                                                          Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDragReference");

        ODPlatformDragReference retval; return retval;
}

SOM_Scope void  SOMLINK ODDragAndDropInitDragAndDrop(ODDragAndDrop *somSelf, 
                                                      Environment *ev, 
                                                     ODSession* session)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropInitDragAndDrop");

}

SOM_Scope ODFacet*  SOMLINK ODDragAndDropFindTargetFacet(ODDragAndDrop *somSelf, 
                                                          Environment *ev, 
                                                         ODPlatformWindow theWindow, 
                                                         PPOINTL mouse, 
                                                         ODPoint* local)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropFindTargetFacet");


        ODFacet* retval; return retval;
}

SOM_Scope void  SOMLINK ODDragAndDropGetPromiseFromDragManager(ODDragAndDrop *somSelf, 
                                                                Environment *ev, 
                                                               ODPart* sourcePart, 
                                                               ODStorageUnitView* destSUView)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetPromiseFromDragManager");

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropGetDataFromDragManager(ODDragAndDrop *somSelf, 
                                                                  Environment *ev, 
                                                                 ODStorageUnitView* theSUView, 
                                                                 ODStorageUnit** renderedSU)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDataFromDragManager");

        return kODFalse;
}

SOM_Scope LinkedList*  SOMLINK ODDragAndDropGetDragItemList(ODDragAndDrop *somSelf, 
                                                             Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDragItemList");


        LinkedList* retval; return retval;
}

SOM_Scope void  SOMLINK ODDragAndDropSetDragItemList(ODDragAndDrop *somSelf, 
                                                      Environment *ev, 
                                                     LinkedList* list)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetDragItemList");

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropGetListFromHandler(ODDragAndDrop *somSelf, 
                                                              Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetListFromHandler");
                  return kODFalse;
}

SOM_Scope void  SOMLINK ODDragAndDropSetListFromHandler(ODDragAndDrop *somSelf, 
                                                         Environment *ev, 
                                                        ODBoolean listFromHandler)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetListFromHandler");

}

SOM_Scope ODFrame*  SOMLINK ODDragAndDropGetSourceFrame(ODDragAndDrop *somSelf, 
                                                         Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetSourceFrame");

        ODFrame* retval; return retval;
}

SOM_Scope void  SOMLINK ODDragAndDropSetSourceFrame(ODDragAndDrop *somSelf, 
                                                     Environment *ev, 
                                                    ODFrame* sourceFrame)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetSourceFrame");

}

SOM_Scope ODULong  SOMLINK ODDragAndDropGetAttributes(ODDragAndDrop *somSelf, 
                                                       Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetAttributes");
               return 0;
}

SOM_Scope void  SOMLINK ODDragAndDropSetAttributes(ODDragAndDrop *somSelf, 
                                                    Environment *ev, 
                                                   ODULong attr)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetAttributes");

}

SOM_Scope ODPart*  SOMLINK ODDragAndDropGetSourcePart(ODDragAndDrop *somSelf, 
                                                       Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetSourcePart");

        ODPart* retval; return retval;
}

SOM_Scope void  SOMLINK ODDragAndDropSetSourcePart(ODDragAndDrop *somSelf, 
                                                    Environment *ev, 
                                                   ODPart* sourcePart)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetSourcePart");

}

SOM_Scope ODDragItemIterator*  SOMLINK ODDragAndDropGetDragItemIterator(ODDragAndDrop *somSelf, 
                                                                         Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDragItemIterator");

        ODDragItemIterator* retval; return retval;
}

SOM_Scope void  SOMLINK ODDragAndDropSetDragItemIterator(ODDragAndDrop *somSelf, 
                                                          Environment *ev, 
                                                         ODDragItemIterator* iter)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetDragItemIterator");

}

SOM_Scope ODSession*  SOMLINK ODDragAndDropGetSession(ODDragAndDrop *somSelf, 
                                                       Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetSession");

        ODSession* retval; return retval;
}

SOM_Scope ODStorageSystem*  SOMLINK ODDragAndDropGetStorageSystem(ODDragAndDrop *somSelf, 
                                                                   Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetStorageSystem");

        ODStorageSystem* retval; return retval;
}

SOM_Scope void  SOMLINK ODDragAndDropSetDragReference(ODDragAndDrop *somSelf, 
                                                       Environment *ev, 
                                                      ODPlatformDragReference dragReference)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetDragReference");

}

SOM_Scope ODFacet*  SOMLINK ODDragAndDropGetPrevFacet(ODDragAndDrop *somSelf, 
                                                       Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetPrevFacet");

        ODFacet* retval; return retval;
}

SOM_Scope void  SOMLINK ODDragAndDropSetPrevFacet(ODDragAndDrop *somSelf, 
                                                   Environment *ev, 
                                                  ODFacet* prevFacet)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetPrevFacet");

}

SOM_Scope ODFacet*  SOMLINK ODDragAndDropGetDestFacet(ODDragAndDrop *somSelf, 
                                                       Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDestFacet");

        ODFacet* retval; return retval;
}

SOM_Scope void  SOMLINK ODDragAndDropSetDestFacet(ODDragAndDrop *somSelf, 
                                                   Environment *ev, 
                                                  ODFacet* destFacet)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetDestFacet");

}

SOM_Scope ODDropResult  SOMLINK ODDragAndDropGetDropResult(ODDragAndDrop *somSelf, 
                                                            Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDropResult");

        ODDropResult retval; return retval;
}

SOM_Scope void  SOMLINK ODDragAndDropSetDropResult(ODDragAndDrop *somSelf, 
                                                    Environment *ev, 
                                                   ODDropResult dropResult)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetDropResult");

}

SOM_Scope ODUShort  SOMLINK ODDragAndDropGetDragOperation(ODDragAndDrop *somSelf, 
                                                           Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDragOperation");

        return 0;
}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropDispatchHandler(ODDragAndDrop *somSelf, 
                                                           Environment *ev, 
                                                          ODEventData* evt, 
                                                          ODFacet* targetfacet)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropDispatchHandler");

        return kODFalse;
}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropCanEmbed(ODDragAndDrop *somSelf, 
                                                    Environment *ev, 
                                                   ODStorageUnit* dropSU)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropCanEmbed");

        return kODFalse;
}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropCanIncorporate(ODDragAndDrop *somSelf, 
                                                          Environment *ev, 
                                                         ODStorageUnit* dropSU, 
                                                         ODType kind)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropCanIncorporate");
        return kODFalse;

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropdmDiscardObject(ODDragAndDrop *somSelf, 
                                                           Environment *ev, 
                                                          ODEventData* evt)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDiscardObject");
        return kODFalse;

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropdmDragError(ODDragAndDrop *somSelf, 
                                                       Environment *ev, 
                                                      ODEventData* evt)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDragError");
        return kODFalse;

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropdmDragLeave(ODDragAndDrop *somSelf, 
                                                       Environment *ev, 
                                                      ODEventData* evt)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDragLeave");
        return kODFalse;

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropdmDragOver(ODDragAndDrop *somSelf, 
                                                      Environment *ev, 
                                                     ODEventData* evt, 
                                                     ODFacet* targetfacet)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDragOver");
        return kODFalse;

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropdmDragOverNotify(ODDragAndDrop *somSelf, 
                                                            Environment *ev, 
                                                           ODEventData* evt)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDragOverNotify");
        return kODFalse;

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropdmDrop(ODDragAndDrop *somSelf, 
                                                  Environment *ev, 
                                                 ODEventData* evt)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDrop");
        return kODFalse;

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropdmDropHelp(ODDragAndDrop *somSelf, 
                                                      Environment *ev, 
                                                     ODEventData* evt)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDropHelp");
        return kODFalse;

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropdmEndConversation(ODDragAndDrop *somSelf, 
                                                             Environment *ev, 
                                                            ODEventData* evt)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmEndConversation");
        return kODFalse;

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropdmPrintObject(ODDragAndDrop *somSelf, 
                                                         Environment *ev, 
                                                        ODEventData* evt)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmPrintObject");
        return kODFalse;

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropdmRender(ODDragAndDrop *somSelf, 
                                                    Environment *ev, 
                                                   ODEventData* evt)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmRender");
        return kODFalse;

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropdmRenderComplete(ODDragAndDrop *somSelf, 
                                                            Environment *ev, 
                                                           ODEventData* evt)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmRenderComplete");
        return kODFalse;

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropdmRenderPrepare(ODDragAndDrop *somSelf, 
                                                           Environment *ev, 
                                                          ODEventData* evt)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmRenderPrepare");
        return kODFalse;

}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropdmDoReceive(ODDragAndDrop *somSelf, 
                                                       Environment *ev, 
                                                      ODPtr pszSelectedRMF, 
                                                      ODUShort usOperation, 
                                                      ODULong hstrRenderToName, 
                                                      ODULong hstrType, 
                                                      ODPtr pszSelectedKind, 
                                                      ODPtr newItem)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDoReceive");
        return kODFalse;

}

SOM_Scope void  SOMLINK ODDragAndDropsomUninit(ODDragAndDrop *somSelf)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropsomUninit");

}

