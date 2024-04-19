//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
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
// @(#) 1.11 com/src/utils/ErDefMap.h, odutils, od96os2, odos29646d 10/23/96 18:47:46 [ 11/15/96 15:50:42 ]

#ifndef _ERDEFMAP_

// Getting the catalog header from ODDebug.h, that's why there is no
// include

#include <ErrorDef.xh>

const int CatIndex[] = {
	// from ErrorDef.xh
	EMSG_50,	// kODErrUndefined -29600
	EMSG_60,	// kODErrAlreadyNotified -29601
	EMSG_70,	// kODErrIllegalNullInput -29602
	EMSG_80,	// kODErrIllegalNullDispatchModuleInput -29603
	EMSG_90,	// kODErrIllegalNullFacetInput -29604
	EMSG_100,	// kODErrIllegalNullFrameInput -29605
	EMSG_110,	// kODErrIllegalNullPartInput -29606
	EMSG_120,	// kODErrIllegalNullTransformInput -29607
	EMSG_130,	// kODErrIllegalNullStorageSystemInput -29608
	EMSG_140,	// kODErrIllegalNullTokenInput -29609
	EMSG_150,	// kODErrIllegalNullShapeInput -29610
	EMSG_160,	// kODErrIllegalNullStorageUnitInput -29611
	EMSG_170,	// kODErrIllegalNullPropertyInput -29612
	EMSG_180,	// kODErrIllegalNullSUCursorInput -29613
	EMSG_190,	// kODErrIllegalNullContainerInput -29614
	EMSG_200,	// kODErrIllegalNullDocumentInput -29615
	EMSG_210,	// kODErrIllegalNullDraftInput -29616
	EMSG_220,	// kODErrIllegalNullValueTypeInput -29617
	EMSG_230,	// kODErrIllegalNullIDInput -29618
	EMSG_240,	// kODErrValueOutOfRange -29619
	EMSG_250,	// kODErrInsufficientInfoInParams -29620
	EMSG_260,	// kODErrObjectNotInitialized -29621
	EMSG_270,	// kODErrOutOfMemory -29622
	EMSG_280,	// kODErrNotImplemented -29623
	EMSG_290,	// kODErrInvalidPersistentFormat -29624
	EMSG_300,	// kODErrSubClassResponsibility -29625
	EMSG_310,	// kODErrUnsupportedExtension -29626
	EMSG_320,	// kODErrInvalidExtension -29627
	EMSG_330,	// kODErrUnknownExtension -29628
	EMSG_340,	// kODErrInvalidObjectType -29629
	EMSG_350,	// kODErrInvalidPersistentObjectID -29630
	EMSG_360,	// kODErrInvalidPersistentObject -29631
	EMSG_370,	// kODErrZeroRefCount -29632
	EMSG_380,	// kODErrRefCountGreaterThanZero -29633
	EMSG_390,	// kODErrRefCountNotEqualOne -29634
	EMSG_400,	// kODErrIteratorOutOfSync -29635
	EMSG_410,	// kODErrIteratorNotInitialized -29636
	EMSG_430,	// kODErrCannotEmbed -29637
	EMSG_440,	// kODErrDoesNotUndo -29638
	EMSG_450,	// kODErrNoPromises -29639
	EMSG_460,	// kODErrDoesNotDrop -29640
	EMSG_470,	// kODErrDoesNotLink -29641
	EMSG_480,	// kODErrPartNotWrapper -29642
	EMSG_490,	// kODErrKeyAlreadyExists -29643
	EMSG_1590,	// kODErrInvalidNSName -29644
	EMSG_1600,	// kODErrInvalidNSType -29645
	EMSG_500,	// kODErrPartInUse -29646
	EMSG_510,	// kODErrInvalidITextFormat -29647
	EMSG_520,	// kODErrInvalidGraphicsSystem -29648
	EMSG_530,	// kODErrNoShapeGeometry -29649
	EMSG_540,	// kODErrTransformErr -29650
	EMSG_550,	// kODErrInvalidPlatformShape -29651
	EMSG_560,	// kODErrCanvasNotFound -29652
	EMSG_570,	// kODErrUnsupportedFramePositionCode -29653
	EMSG_580,	// kODErrInvalidFacet -29654
	EMSG_590,	// kODErrFacetNotFound -29655
	EMSG_600,	// kODErrCanvasHasNoOwner -29656
	EMSG_610,	// kODErrNotRootFrame -29657
	EMSG_620,	// kODErrIllegalRecursiveEmbedding -29658
	EMSG_630,	// kODErrInvalidFrame -29659
	EMSG_640,	// kODErrFrameHasFacets -29660
	EMSG_660,	// kODErrInvalidBlock -29661
	EMSG_670,	// kODErrNotAnODToken -29662
	EMSG_680,	// kODErrCantCountFromLists -29663
	EMSG_690,	// kODErrUnsupportedPosCode -29664
	EMSG_700,	// kODErrInvalidPermissions -29665
	EMSG_710,	// kODErrCannotCreateContainer -29666
	EMSG_720,	// kODErrCannotOpenContainer -29667
	EMSG_740,	// kODErrContainerDoesNotExist -29668
	EMSG_750,	// kODErrDocumentDoesNotExist -29669
	EMSG_760,	// kODErrDraftDoesNotExist -29670
	EMSG_770,	// kODErrDraftHasBeenDeleted -29671
	EMSG_780,	// kODErrInvalidStorageUnit -29672
	EMSG_790,	// kODErrIllegalOperationOnSU -29673
	EMSG_800,	// kODErrSUValueDoesNotExist -29674
	EMSG_810,	// kODErrIllegalNonTopmostDraft -29675
	EMSG_830,	// kODErrNoValueAtThatIndex -29676
	EMSG_840,	// kODErrCannotAddProperty -29677
	EMSG_850,	// kODErrUnfocusedStorageUnit -29678
	EMSG_860,	// kODErrInvalidStorageUnitRef -29679
	EMSG_820,	// kODErrNoSysTranslationFacility -29680
	EMSG_870,	// kODErrInvalidStorageUnitKey -29681
	EMSG_880,	// kODErrStorageUnitNotLocked -29682
	EMSG_890,	// kODErrInvalidDraftKey -29683
	EMSG_900,	// kODErrCloningInProgress -29684
	EMSG_910,	// kODErrValueIndexOutOfRange -29685
	EMSG_920,	// kODErrInvalidValueType -29686
	EMSG_930,	// kODErrIllegalPropertyName -29687
	EMSG_940,	// kODErrPropertyDoesNotExist -29688
	EMSG_950,	// kODErrNoDraftProperties -29689
	EMSG_960,	// kODErrCannotCreateFrame -29690
	EMSG_970,	// kODErrCannotAcquireFrame -29691
	EMSG_980,	// kODErrCannotCreatePart -29692
	EMSG_990,	// kODErrCannotAcquirePart -29693
	EMSG_1010,	// kODErrCannotCreateLink -29694
	EMSG_1020,	// kODErrCannotAcquireLink -29695
	EMSG_1030,	// kODErrInvalidID -29696
	EMSG_1040,	// kODErrInconsistentCloneKind -29697
	EMSG_1050,	// kODErrInvalidCloneKind -29698
	EMSG_1060,	// kODErrInvalidDestinationDraft -29699
	EMSG_1070,	// kODErrMoveIntoSelf -29700
	EMSG_1080,	// kODErrNullDestinationFrame -29701
	EMSG_1090,	// kODErrInvalidBelowDraft -29702
	0,		// no -29703
	EMSG_1110,	// kODErrCannotCollapseDrafts -29704
	EMSG_1120,	// kODErrNonEmptyDraft -29705
	EMSG_1130,	// kODErrNoPreviousDraft -29706
	EMSG_1140,	// kODErrOutstandingDraft -29707
	EMSG_1150,	// kODErrInvalidDraftID -29708
	EMSG_1160,	// kODErrCannotChangePermissions -29709
	EMSG_1170,	// kODErrContainerExists -29710
	0,		// no -29711
	EMSG_1180,	// kODErrCannotGetExternalLink -29712
	EMSG_1190,	// kODErrNoLinkSpecValue -29713
	EMSG_1200,	// kODErrUnknownLinkSpecVersion -29714
	EMSG_1210,	// kODErrCorruptLinkSpecValue -29715
	EMSG_1220,	// kODErrNotExportedLink -29716
	EMSG_1230,	// kODErrBrokenLink -29717
	EMSG_1240,	// kODErrCannotRevealLink -29718
	EMSG_1250,	// kODErrCorruptLink -29719
	EMSG_1260,	// kODErrLinkAlreadyExported -29720
	EMSG_1270,	// kODErrNoLinkContent -29721
	EMSG_1280,	// kODErrCannotRegisterDependent -29722
	EMSG_1290,	// kODErrNotImportedLink -29723
	EMSG_1300,	// kODErrInvalidLinkKey -29724
	EMSG_1310,	// kODErrBrokenLinkSource -29725
	EMSG_1320,	// kODErrCorruptLinkSource -29726
	EMSG_1330,	// kODErrCannotFindLinkSourceEdition -29727
	EMSG_1340,	// kODErrCannotFindLinkSource -29728
	EMSG_1350,	// kODErrAlreadyImportedLink -29729
	EMSG_1360,	// kODErrUnknownUpdateID -29730
	EMSG_1370,	// kODErrCannotEstablishLink -29731
	EMSG_1380,	// kODErrNoEditionManager -29732
	EMSG_1390,	// kODErrDocNotSaved -29733
	EMSG_1400,	// kODErrNullFacetInput -29734
	EMSG_1410,	// kODErrNullLinkInfoInput -29735
	EMSG_1420,	// kODErrNullLinkInfoResultInput -29736
	EMSG_1430,	// kODErrNullPasteAsResultInput -29737
	EMSG_1440,	// kODErrNoDragManager -29738
	EMSG_1450,	// kODErrNoDragSystemStorage -29739
	EMSG_1460,	// kODErrDragItemNotFound -29740
	EMSG_1470,	// kODErrCannotAllocateDragItem -29741
	EMSG_1480,	// kODErrUnknownDragImageType -29742
	EMSG_1490,	// kODErrDragTrackingException -29743
	EMSG_1500,	// kODErrBackgroundClipboardClear -29744
	EMSG_1510,	// kODErrIllegalClipboardCloneKind -29745
	EMSG_1520,	// kODErrFocusAlreadyRegistered -29746
	EMSG_1530,	// kODErrFocusNotRegistered -29747
	0,		// no -29748
	EMSG_1540,	// kODErrCannotMarkAction -29749
	EMSG_1550,	// kODErrEmptyStack -29750
	EMSG_1560,	// kODErrNoBeginAction -29751
	EMSG_1570,	// kODErrCannotAddAction -29752
	EMSG_1580,	// kODErrCannotCreateWindow -29753
	EMSG_650,	// kODErrInvalidLinkStatus -29754
	EMSG_420,	// kODErrInvalidIterator -29755
	EMSG_730,	// kODErrFatalContainerError -29756
	0,		// no -29757
	EMSG_10,	// kODErrFacetChangeToOrFromOnscreenDynamic -29758
	EMSG_20,	// kODErrInvalidParameter -29759
	EMSG_30,	// kODErrOnlyOwnerCanChangeCanvas -29760
	EMSG_1610,	// kODErrCannotExportToIDataObject -29761
	EMSG_2000,	// kODErrOpeningMemContainer -29762
	EMSG_1640,	// kODErrNotEnoughSubmenuIDs -29763
	EMSG_1650,	// kODErrDuplicateID -29764
	EMSG_1660,	// kODErrUnknownID -29765
	EMSG_2010,	// kODErrClosingNonODWindow -29766
	EMSG_2020,	// kODErrNoDraftInWindow -29767
	EMSG_2030,	// kODErrRegistrationError -29768
	EMSG_2040,	// kODErrRegStartProcessFail -29769
	EMSG_2050,	// kODErrEnvVariableNotSet -29770
	EMSG_1970,	// kODErrDispatcherNotInitialized -29771
	EMSG_1980,	// kODErrExistingDispatchModule -29772
	EMSG_1990,	// kODErrInvalidDispatchModule -29773
	EMSG_2410,	// kODErrWrongPlatformForODZ -29774
	EMSG_1471,	// kODErrDropFacetNotKnown -29775
	0, 0, 0, 0, 0,  // no -29776 through -29788
	0, 0, 0, 0, 0, 0, 0, 0,
	// From BentoDef.h
	EMSG_1670,	// kODErrNoDocumentProperties -29789
	EMSG_1680,	// kODErrCannotGetDraftVersion -29790
	EMSG_1690,	// kODErrInvalidZone -29791
	EMSG_1700,	// kODErrNotPromise -29792
	EMSG_1710,	// kODErrNoVersionList -29793
	EMSG_1720,	// kODErrDraftExistsWithDifferentID -29794
	EMSG_1730,	// kODErrBentoInvalidObject -29795
	EMSG_1740,	// kODErrBentoCannotOpenContainer -29796
	EMSG_1750,	// kODErrBentoCannotCreateContainer -29797
	EMSG_1760,	// kODErrBentoCannotNewValue -29798
	EMSG_1770,	// kODErrBentoCannotNewObject -29799
	EMSG_1780,	// kODErrBentoInvalidType -29800
	EMSG_1790,	// kODErrBentoInvalidProperty -29801
	EMSG_1795,	// kODErrBentoNoSession -29802
	EMSG_1800,	// kODErrInvalidIDList -29803
	0,		// no -29804
	EMSG_1810,	// kODErrSemaphoreReleased -29805
	EMSG_1820,	// kODErrVersionListUnavailable -29806
	EMSG_1830,	// kODErrCannotCreateDraftVersion -29807
	EMSG_1840,	// kODErrInvalidVersion -29808
	EMSG_1850,	// kODErrBentoErr -29809
	0,		// no -29810 through -29838
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	// From UtilErrs.h
	EMSG_1960,	// kODErrEndOfFile -29839
	EMSG_1860,	// kODErrShapeTooComplex -29840
	EMSG_1870,	// kODErrInvalidKey -29841
	EMSG_1880,	// kODErrHashValueSizeTooBig -29842
	EMSG_1890,	// kODErrWriteErr -29843
	EMSG_1900,	// kODErrReadErr -29844
	EMSG_1910,	// kODErrAssertionFailed -29845
	EMSG_1920,	// kODErrCantLoadSOMClass -29846
	EMSG_1930,	// kODErrFileOpened -29847
	EMSG_1940,	// kODErrFileClosed -29848
	EMSG_1950,	// kODErrSOMException -29849
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    // no -29850 through -29859
	// from ErrorDef.xh
	EMSG_2060,	// kODError -29860
	EMSG_2080,	// kODErrLinkSourceIsEmpty -29861
	EMSG_2090,	// kODErrLinkSourceAlreadyInited -29862
	EMSG_2100,	// kODErrLinkSourceConnected -29863
	EMSG_2110,	// kODErrInvalidTargetID -29864
	EMSG_2120,	// kODErrRequestMutexTimeOut -29865
	EMSG_2130,	// kODErrRequestMutexInterrupt -29866
	EMSG_2140,	// kODErrRequestMutexInvalidHandle -29867
	EMSG_2150,	// kODErrRequestMutexTooManyRequests -29868
	EMSG_2160,	// kODErrRequestMutexOwnerDied -29869
	EMSG_2170,	// kODErrReleaseMutexInvalidHandle -29870
	EMSG_2180,	// kODErrReleaseMutexNotOwner -29871
	EMSG_2190,	// kODErrDocNotRegistered -29872
	EMSG_2200,	// kODErrDocNotOpened -29873
	EMSG_2210,	// kODErrLinkNotRegistered -29874
	EMSG_2220,	// kODErrLinkNotOpened -29875
	EMSG_2230,	// kODErrInvalidDocPathName -29876
	EMSG_2240,	// kODErrDocAlreadyOpened -29877
	EMSG_2250,	// kODErrInvalidDocID -29878
	EMSG_2260,	// kODErrInvalidLinkID -29879
	EMSG_2270,	// kODErrInvalidAvlSvr -29880
	EMSG_2280,	// kODErrInvalidLinkSource -29881
	EMSG_2290,	// kODErrInvalidAuxState -29882
	EMSG_2300,	// kODErrLinkIDNotInDoc -29883
	EMSG_2310,	// kODErrInvalidShell -29884
	EMSG_2320,	// kODErrLinkAlreadyOpened -29885
	EMSG_2330,	// kODErrWaitingUnlock -29886
	EMSG_2340,	// kODErrDocOpenedFailed -29887
	EMSG_2350,	// kODErrLinkOpenedFailed -29888
	EMSG_2360,	// kODErrInvalidLink -29889
	EMSG_2370,	// kODErrNoLinkSource -29890
	EMSG_2380,	// kODErrSourceDocRemote -29891
	EMSG_2390,	// kODErrSourceDocNotOpen -29892
	EMSG_2400	// kODErrInvalidDraft -29893
};


const char CatDefaultMsg [(kODMaxError - kODMinError) + 1] [55] = {
	// from ErrorDef.xh
	{ "Undefined error" },	// EMSG_50 - kODErrUndefined -29600
	{ "Already notified" },	// EMSG_60 - kODErrAlreadyNotified -29601
	{ "Illegal null input" },	// EMSG_70 - kODErrIllegalNullInput -29602
	{ "Illegal null dispatch module input" },	// EMSG_80 - kODErrIllegalNullDispatchModuleInput -29603
	{ "Illegal null facet input" },	// EMSG_90 - kODErrIllegalNullFacetInput -29604
	{ "Illegal null frame input" },	// EMSG_100 - kODErrIllegalNullFrameInput -29605
	{ "Illegal null part input" },	// EMSG_110 - kODErrIllegalNullPartInput -29606
	{ "Illegal null transform input" },	// EMSG_120 - kODErrIllegalNullTransformInput -29607
	{ "Illegal null storage system input" },	// EMSG_130 - kODErrIllegalNullStorageSystemInput -29608
	{ "Illegal null token input" },	// EMSG_140 - kODErrIllegalNullTokenInput -29609
	{ "Illegal null shape input" },	// EMSG_150 - kODErrIllegalNullShapeInput -29610
	{ "Illegal null storage unit input" },	// EMSG_160 - kODErrIllegalNullStorageUnitInput -29611
	{ "Illegal null property input" },	// EMSG_170 - kODErrIllegalNullPropertyInput -29612
	{ "Illegal null storage unit cursor input" },	// EMSG_180 - kODErrIllegalNullSUCursorInput -29613
	{ "Illegal null container input" },	// EMSG_190 - kODErrIllegalNullContainerInput -29614
	{ "Illegal null document input" },	// EMSG_200 - kODErrIllegalNullDocumentInput -29615
	{ "Illegal null draft input" },	// EMSG_210 - kODErrIllegalNullDraftInput -29616
	{ "Illegal null Value type input" },	// EMSG_220 - kODErrIllegalNullValueTypeInput -29617
	{ "Illegal null ID input" },	// EMSG_230 - kODErrIllegalNullIDInput -29618
	{ "Value out of range" },	// EMSG_240 - kODErrValueOutOfRange -29619
	{ "Insufficient info in parameters" },	// EMSG_250 - kODErrInsufficientInfoInParams -29620
	{ "Object not initialized" },	// EMSG_260 - kODErrObjectNotInitialized -29621
	{ "Out of memory" },	// EMSG_270 - kODErrOutOfMemory -29622
	{ "Not implemented" },	// EMSG_280 - kODErrNotImplemented -29623
	{ "Invalid persistent format" },	// EMSG_290 - kODErrInvalidPersistentFormat -29624
	{ "Subclass responsibility" },	// EMSG_300 - kODErrSubClassResponsibility -29625
	{ "Unsupported extension" },	// EMSG_310 - kODErrUnsupportedExtension -29626
	{ "Invalid extension" },	// EMSG_320 - kODErrInvalidExtension -29627
	{ "Unknown extension" },	// EMSG_330 - kODErrUnknownExtension -29628
	{ "Invalid object type" },	// EMSG_340 - kODErrInvalidObjectType -29629
	{ "Invalid persistent object identifier" },	// EMSG_350 - kODErrInvalidPersistentObjectID -29630
	{ "Invalid persistent object" },	// EMSG_360 - kODErrInvalidPersistentObject -29631
	{ "Zero reference count" },	// EMSG_370 - kODErrZeroRefCount -29632
	{ "Refence count greater than zero" },	// EMSG_380 - kODErrRefCountGreaterThanZero -29633
	{ "Refence count not equal to one" },	// EMSG_390 - kODErrRefCountNotEqualOne -29634
	{ "Iterator out of synchronization" },	// EMSG_400 - kODErrIteratorOutOfSync -29635
	{ "Iterator not initialized" },	// EMSG_410 - kODErrIteratorNotInitialized -29636
	{ "Cannot embed" },	// EMSG_430 - kODErrCannotEmbed -29637
	{ "Does not undo" },	// EMSG_440 - kODErrDoesNotUndo -29638
	{ "No promises" },	// EMSG_450 - kODErrNoPromises -29639
	{ "Does not drop" },	// EMSG_460 - kODErrDoesNotDrop -29640
	{ "Does not link" },	// EMSG_470 - kODErrDoesNotLink -29641
	{ "Part not wrapper" },	// EMSG_480 - kODErrPartNotWrapper -29642
	{ "Key already exists" },	// EMSG_490 - kODErrKeyAlreadyExists -29643
	{ "Invalid name space name" },	// EMSG_1590 - kODErrInvalidNSName -29644
	{ "Invalid name space type" },	// EMSG_1600 - kODErrInvalidNSType -29645
	{ "Part in use" },	// EMSG_500 - kODErrPartInUse -29646
	{ "Invalid IText format" },	// EMSG_510 - kODErrInvalidITextFormat -29647
	{ "Invalid graphics system" },	// EMSG_520 - kODErrInvalidGraphicsSystem -29648
	{ "No shape geometry" },	// EMSG_530 - kODErrNoShapeGeometry -29649
	{ "Transform error" },	// EMSG_540 - kODErrTransformErr -29650
	{ "Invalid platform shape" },	// EMSG_550 - kODErrInvalidPlatformShape -29651
	{ "Canvas not found" },	// EMSG_560 - kODErrCanvasNotFound -29652
	{ "Unsupported frame position code" },	// EMSG_570 - kODErrUnsupportedFramePositionCode -29653
	{ "Invalid facet" },	// EMSG_580 - kODErrInvalidFacet -29654
	{ "Facet not found" },	// EMSG_590 - kODErrFacetNotFound -29655
	{ "Canvas has no owner" },	// EMSG_600 - kODErrCanvasHasNoOwner -29656
	{ "Not root frame" },	// EMSG_610 - kODErrNotRootFrame -29657
	{ "Illegal recursive embedding" },	// EMSG_620 - kODErrIllegalRecursiveEmbedding -29658
	{ "Invalid frame" },	// EMSG_630 - kODErrInvalidFrame -29659
	{ "Frame has facets" },	// EMSG_640 - kODErrFrameHasFacets -29660
	{ "Invalid block" },	// EMSG_660 - kODErrInvalidBlock -29661
	{ "Not an ODToken" },	// EMSG_670 - kODErrNotAnODToken -29662
	{ "Cannot count from lists" },	// EMSG_680 - kODErrCantCountFromLists -29663
	{ "Unsupported position code" },	// EMSG_690 - kODErrUnsupportedPosCode -29664
	{ "Invalid permissions" },	// EMSG_700 - kODErrInvalidPermissions -29665
	{ "Cannot create container" },	// EMSG_710 - kODErrCannotCreateContainer -29666
	{ "Cannot open container" },	// EMSG_720 - kODErrCannotOpenContainer -29667
	{ "Container does not exist" },	// EMSG_740 - kODErrContainerDoesNotExist -29668
	{ "Document does not exist" },	// EMSG_750 - kODErrDocumentDoesNotExist -29669
	{ "Draft does not exist" },	// EMSG_760 - kODErrDraftDoesNotExist -29670
	{ "Draft has been deleted" },	// EMSG_770 - kODErrDraftHasBeenDeleted -29671
	{ "Invalid storage unit" },	// EMSG_780 - kODErrInvalidStorageUnit -29672
	{ "Illegal operation on storage unit" },	// EMSG_790 - kODErrIllegalOperationOnSU -29673
	{ "Storage unit value does not exist" },	// EMSG_800 - kODErrSUValueDoesNotExist -29674
	{ "Illegal non-topmost draft" },	// EMSG_810 - kODErrIllegalNonTopmostDraft -29675
	{ "No value at that index" },	// EMSG_830 - kODErrNoValueAtThatIndex -29676
	{ "Cannot add property" },	// EMSG_840 - kODErrCannotAddProperty -29677
	{ "Unfocused storage unit" },	// EMSG_850 - kODErrUnfocusedStorageUnit -29678
	{ "Invalid storage unit reference" },	// EMSG_860 - kODErrInvalidStorageUnitRef -29679
	{ "No system data translation facility" },	// EMSG_820 - kODErrNoSysTranslationFacility -29680
	{ "Invalid storage unit key" },	// EMSG_870 - kODErrInvalidStorageUnitKey -29681
	{ "Storage unit not locked" },	// EMSG_880 - kODErrStorageUnitNotLocked -29682
	{ "Invalid draft key" },	// EMSG_890 - kODErrInvalidDraftKey -29683
	{ "Cloning in progress" },	// EMSG_900 - kODErrCloningInProgress -29684
	{ "Value index out of range" },	// EMSG_910 - kODErrValueIndexOutOfRange -29685
	{ "Invalid value type" },	// EMSG_920 - kODErrInvalidValueType -29686
	{ "Illegal property name" },	// EMSG_930 - kODErrIllegalPropertyName -29687
	{ "Property does not exist" },	// EMSG_940 - kODErrPropertyDoesNotExist -29688
	{ "No draft properties" },	// EMSG_950 - kODErrNoDraftProperties -29689
	{ "Cannot create frame" },	// EMSG_960 - kODErrCannotCreateFrame -29690
	{ "Cannot acquire frame" },	// EMSG_970 - kODErrCannotAcquireFrame -29691
	{ "Cannot create part" },	// EMSG_980 - kODErrCannotCreatePart -29692
	{ "Cannot acquire part" },	// EMSG_990 - kODErrCannotAcquirePart -29693
	{ "Cannot create link" },	// EMSG_1010 - kODErrCannotCreateLink -29694
	{ "Cannot acquire link" },	// EMSG_1020 - kODErrCannotAcquireLink -29695
	{ "Invalid identifier" },	// EMSG_1030 - kODErrInvalidID -29696
	{ "Inconsistent clone kind" },	// EMSG_1040 - kODErrInconsistentCloneKind -29697
	{ "Invalid clone kind" },	// EMSG_1050 - kODErrInvalidCloneKind -29698
	{ "Invalid destination draft" },	// EMSG_1060 - kODErrInvalidDestinationDraft -29699
	{ "Move into self" },	// EMSG_1070 - kODErrMoveIntoSelf -29700
	{ "Null destination frame" },	// EMSG_1080 - kODErrNullDestinationFrame -29701
	{ "Invalid below draft" },	// EMSG_1090 - kODErrInvalidBelowDraft -29702
	{ "" },		// no -29703
	{ "Cannot collapse drafts" },	// EMSG_1110 - kODErrCannotCollapseDrafts -29704
	{ "Not an empty draft" },	// EMSG_1120 - kODErrNonEmptyDraft -29705
	{ "No previous draft" },	// EMSG_1130 - kODErrNoPreviousDraft -29706
	{ "Outstanding draft" },	// EMSG_1140 - kODErrOutstandingDraft -29707
	{ "Invalid draft identifier" },	// EMSG_1150 - kODErrInvalidDraftID -29708
	{ "Cannot change permissions" },	// EMSG_1160 - kODErrCannotChangePermissions -29709
	{ "Container exists" },	// EMSG_1170 - kODErrContainerExists -29710
	{ "" },		// no -29711
	{ "Cannot get external link" },	// EMSG_1180 - kODErrCannotGetExternalLink -29712
	{ "No link specification value" },	// EMSG_1190 - kODErrNoLinkSpecValue -29713
	{ "Unknown link specification version" },	// EMSG_1200 - kODErrUnknownLinkSpecVersion -29714
	{ "Corrupt link specification value" },	// EMSG_1210 - kODErrCorruptLinkSpecValue -29715
	{ "Not exported link" },	// EMSG_1220 - kODErrNotExportedLink -29716
	{ "Broken link" },	// EMSG_1230 - kODErrBrokenLink -29717
	{ "Cannot reveal link" },	// EMSG_1240 - kODErrCannotRevealLink -29718
	{ "Corrupt link" },	// EMSG_1250 - kODErrCorruptLink -29719
	{ "Link already exported" },	// EMSG_1260 - kODErrLinkAlreadyExported -29720
	{ "No link content" },	// EMSG_1270 - kODErrNoLinkContent -29721
	{ "Cannot register dependent" },	// EMSG_1280 - kODErrCannotRegisterDependent -29722
	{ "Not imported link" },	// EMSG_1290 - kODErrNotImportedLink -29723
	{ "Invalid link key" },	// EMSG_1300 - kODErrInvalidLinkKey -29724
	{ "Broken link source" },	// EMSG_1310 - kODErrBrokenLinkSource -29725
	{ "Corrupt link source" },	// EMSG_1320 - kODErrCorruptLinkSource -29726
	{ "Cannot find link source edition" },	// EMSG_1330 - kODErrCannotFindLinkSourceEdition -29727
	{ "Cannot find link source" },	// EMSG_1340 - kODErrCannotFindLinkSource -29728
	{ "Already imported link" },	// EMSG_1350 - kODErrAlreadyImportedLink -29729
	{ "Unknown update identifier" },	// EMSG_1360 - kODErrUnknownUpdateID -29730
	{ "Cannot establish link" },	// EMSG_1370 - kODErrCannotEstablishLink -29731
	{ "No edition manager" },	// EMSG_1380 - kODErrNoEditionManager -29732
	{ "Document not saved" },	// EMSG_1390 - kODErrDocNotSaved -29733
	{ "Null facet input" },	// EMSG_1400 - kODErrNullFacetInput -29734
	{ "Null link information input" },	// EMSG_1410 - kODErrNullLinkInfoInput -29735
	{ "Null link information result input" },	// EMSG_1420 - kODErrNullLinkInfoResultInput -29736
	{ "Null paste as result input" },	// EMSG_1430 - kODErrNullPasteAsResultInput -29737
	{ "No drag manager" },	// EMSG_1440 - kODErrNoDragManager -29738
	{ "No drag system storage" },	// EMSG_1450 - kODErrNoDragSystemStorage -29739
	{ "Drag item not found" },	// EMSG_1460 - kODErrDragItemNotFound -29740
	{ "Cannot allocate drag item" },	// EMSG_1470 - kODErrCannotAllocateDragItem -29741
	{ "Unknown drag image type" },	// EMSG_1480 - kODErrUnknownDragImageType -29742
	{ "Drag tracking exception" },	// EMSG_1490 - kODErrDragTrackingException -29743
	{ "Background clipboard clear" },	// EMSG_1500 - kODErrBackgroundClipboardClear -29744
	{ "Illegal clipboard clone kind" },	// EMSG_1510 - kODErrIllegalClipboardCloneKind -29745
	{ "Focus already registered" },	// EMSG_1520 - kODErrFocusAlreadyRegistered -29746
	{ "Focus not registered" },	// EMSG_1530 - kODErrFocusNotRegistered -29747
	{ "" },		// no -29748
	{ "Cannot mark action" },	// EMSG_1540 - kODErrCannotMarkAction -29749
	{ "Empty stack" },	// EMSG_1550 - kODErrEmptyStack -29750
	{ "No begin action" },	// EMSG_1560 - kODErrNoBeginAction -29751
	{ "Cannot add action" },	// EMSG_1570 - kODErrCannotAddAction -29752
	{ "Cannot create window" },	// EMSG_1580 - kODErrCannotCreateWindow -29753
	{ "Invalid link status" },	// EMSG_650 - kODErrInvalidLinkStatus -29754
	{ "Invalid iterator" },	// EMSG_420 - kODErrInvalidIterator -29755
	{ "Fatal container error" },	// EMSG_730 - kODErrFatalContainerError -29756
	{ "" },		// no -29757
	{ "Facet change to or from on sreen dynamic" },	// EMSG_10 - kODErrFacetChangeToOrFromOnscreenDynamic -29758
	{ "Invalid parameter" },	// EMSG_20 - kODErrInvalidParameter -29759
	{ "Only owner can change canvas" },	// EMSG_30 - kODErrOnlyOwnerCanChangeCanvas -29760
	{ "Cannot export to IData object" },	// EMSG_1610 - kODErrCannotExportToIDataObject -29761
	{ "Document Manager: Error opening memory container" },	// EMSG_2000 - kODErrOpeningMemContainer -29762
	{ "Not enough submenu identifiers" },	// EMSG_1640 - kODErrNotEnoughSubmenuIDs -29763
	{ "Duplicate identifier" },	// EMSG_1650 - kODErrDuplicateID -29764
	{ "Unknown identifier" },	// EMSG_1660 - kODErrUnknownID -29765
	{ "Document Manager: Error closing non-OpenDoc window" },	// EMSG_2010 - kODErrClosingNonODWindow -29766
	{ "Document Manager: No draft in window" },	// EMSG_2020 - kODErrNoDraftInWindow -29767
	{ "Registration error" },	// EMSG_2030 - kODErrRegistrationError -29768
	{ "Registration: Start process failed" },	// EMSG_2040 - kODErrRegStartProcessFail -29769
	{ "Environment variable not set" },	// EMSG_2050 - kODErrEnvVariableNotSet -29770
	{ "Dispatcher not initialized" },	// EMSG_1970 - kODErrDispatcherNotInitialized -29771
	{ "Existing dispatch module" },	// EMSG_1980 - kODErrExistingDispatchModule -29772
	{ "Invalid dispatch module" },	// EMSG_1990 - kODErrInvalidDispatchModule -29773
	{ "The ODZ package file cannot be used on this platform." },	// EMSG_2410 - kODErrWrongPlatformForODZ -29774
	{ "Facet receiving drop undetermined." },	// EMSG_1471 - kODErrDropFacetNotKnown -29775
	// no -29776 through -29788
	{ "" }, { "" }, { "" }, { "" }, { "" },
	{ "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" },
	// From BentoDef.h
	{ "No document properties" },	// EMSG_1670 - kODErrNoDocumentProperties -29789
	{ "Cannot get draft version" },	// EMSG_1680 - kODErrCannotGetDraftVersion -29790
	{ "Invalid zone" },	// EMSG_1690 - kODErrInvalidZone -29791
	{ "Not a promise" },	// EMSG_1700 - kODErrNotPromise -29792
	{ "No version list" },	// EMSG_1710 - kODErrNoVersionList -29793
	{ "A draft exists with a different identifier" },	// EMSG_1720 - kODErrDraftExistsWithDifferentID -29794
	{ "Bento found an invalid object" },	// EMSG_1730 - kODErrBentoInvalidObject -29795
	{ "Bento cannot open container" },	// EMSG_1740 - kODErrBentoCannotOpenContainer -29796
	{ "Bento cannot create container" },	// EMSG_1750 - kODErrBentoCannotCreateContainer -29797
	{ "Bento cannot create a new value" },	// EMSG_1760 - kODErrBentoCannotNewValue -29798
	{ "Bento cannot create a new object" },	// EMSG_1770 - kODErrBentoCannotNewObject -29799
	{ "Bento found an invalid type" },	// EMSG_1780 - kODErrBentoInvalidType -29800
	{ "Bento found an invalid property" },	// EMSG_1790 - kODErrBentoInvalidProperty -29801
	{ "Bento has no session" },	// EMSG_1795 - kODErrBentoNoSession -29802
	{ "Invalid identifier list" },	// EMSG_1800 - kODErrInvalidIDList -29803
	{ "" },		// no -29804
	{ "Semaphore released" },	// EMSG_1810 - kODErrSemaphoreReleased -29805
	{ "Version list is unavailable" },	// EMSG_1820 - kODErrVersionListUnavailable -29806
	{ "Cannot create a draft version" },	// EMSG_1830 - kODErrCannotCreateDraftVersion -29807
	{ "Invalid version" },	// EMSG_1840 - kODErrInvalidVersion -29808
	{ "Bento error" },	// EMSG_1850 - kODErrBentoErr -29809
	{ "" },		// no -29810 through -29838
	{ "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" },
	{ "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" },
	{ "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" },
	// From UtilErrs.h
	{ "Internal error: End of file" },	// EMSG_1960 kODErrEndOfFile -29839
	{ "Shape too complex" },	// EMSG_1860 - kODErrShapeTooComplex -29840
	{ "Invalid key" },	// EMSG_1870 - kODErrInvalidKey -29841
	{ "Hash value size too big" },	// EMSG_1880 - kODErrHashValueSizeTooBig -29842
	{ "Write error" },	// EMSG_1890 - kODErrWriteErr -29843
	{ "Read error" },	// EMSG_1900 - kODErrReadErr -29844
	{ "Assertion failed" },	// EMSG_1910 - kODErrAssertionFailed -29845
	{ "Cannot load SOM class" },	// EMSG_1920 - kODErrCantLoadSOMClass -29846
	{ "File opened" },	// EMSG_1930 - kODErrFileOpened -29847
	{ "File closed" },	// EMSG_1940 - kODErrFileClosed -29848
	{ "SOM exception" },	// EMSG_1950 - kODErrSOMException -29849
	{ "" },		// no -29850 through -29859
	{ "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" }, { "" },
	{ "Linking error" },	// EMSG_2060 - kODError -29860
	{ "Link source is empty" },	// EMSG_2080 - kODErrLinkSourceAlreadyInited -29861
	{ "Link source already initialized" },	// EMSG_2090 - kODErrLinkSourceAlreadyInited -29862
	{ "Link source connected" },	// EMSG_2100 - kODErrLinkSourceConnected -29863
	{ "Invalid target ID" },	// EMSG_2110 - kODErrInvalidTargetID -29864
	{ "Request Mutex timed out" },	// EMSG_2120 - kODErrRequestMutexTimeOut -29865
	{ "Request Mutex interrupt" },	// EMSG_2130 - kODErrRequestMutexInterrupt -29866
	{ "Request Mutex invalid handle" },	// EMSG_2140 - kODErrRequestMutexInvalidHandle -29867
	{ "Request Mutex too many requests" },	// EMSG_2150 - kODErrRequestMutexTooManyRequests -29868
	{ "Request Mutex owner died" },	// EMSG_2160 - kODErrRequestMutexOwnerDied -29869
	{ "Release Mutex invalid handle" },	// EMSG_2170 - cwkODErrReleaseMutexInvalidHandlex -29870
	{ "Release Mutex not owner" },	// EMSG_2180 - kODErrReleaseMutexNotOwner -29871
	{ "Document not registered" },	// EMSG_2190 - kODErrDocNotRegistered -29872
	{ "Document not opened" },	// EMSG_2200 - kODErrDocNotOpened -29873
	{ "Link not registered" },	// EMSG_2210 - kODErrLinkNotRegistered -29874
	{ "Link not opened" },	// EMSG_2220 - kODErrLinkNotOpened -29875
	{ "Invalid document path name" },	// EMSG_2230 - kODErrInvalidDocPathName -29876
	{ "Document already opened" },	// EMSG_2240 - kODErrDocAlreadyOpened -29877
	{ "Invalid document ID" },	// EMSG_2250 - kODErrInvalidDocID -29878
	{ "Invalid link ID" },	// EMSG_2260 - kODErrInvalidAvlSvr -29879
	{ "Invalid avl server" },	// EMSG_2270 - kODErrInvalidAvlSvr -29880
	{ "Invalid link source" },	// EMSG_2280 - kODErrInvalidLinkSource -29881
	{ "Invalid aux state" },	// EMSG_2290 - kODErrInvalidAuxState -29882
	{ "Link ID not in document" },	// EMSG_2300 - kODErrLinkIDNotInDoc -29883
	{ "Invalid shell" },	// EMSG_2310 - kODErrInvalidShell -29884
	{ "Link already opened" },	// EMSG_2320 - kODErrLinkAlreadyOpened -29885
	{ "Waiting unlock" },	// EMSG_2330 - kODErrWaitingUnlock -29886
	{ "Document open failed" },	// EMSG_2340 - kODErrDocOpenedFailed -29887
	{ "Link open failed" },	// EMSG_2350 - kODErrLinkOpenedFailed -29888
	{ "Invalid link" },	// EMSG_2360 - kODErrInvalidLink -29889
	{ "No link source" },	// EMSG_2370 - kODErrNoLinkSource -29890
	{ "Source document remote" },	// EMSG_2380 - kODErrSourceDocRemote -29891
	{ "Source document not open" },	// EMSG_2390 - kODErrSourceDocNotOpen -29892
	{ "Invalid draft" }	// EMSG_2400 - kODErrInvalidDraft -29893
};

#endif
