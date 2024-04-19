/* @(#)Z 1.2 os2/src/avsshell/dsres.h, oddataxfer, od96os2, odos29646d 96/11/15 15:52:47 (96/08/23 01:40:45) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: oddataxfer
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
#include "cmddefs.xh"

#define ID_WINDOW             50

#define ROOT_SHELLMENU_RESID  101

#define ROOT_DRAFTS_RESID     102
#define POPUP_DRAFTS_RESID    103

#define MAX_STR_LEN           100      // max length of string table entry.
#define MAX_PATH_STR_LEN      (MAX_STR_LEN + CCHMAXPATHCOMP)     // max len of str with path comp.

#define IDS_INVALID_BENTO            100    // start string table equates
#define IDS_FILE_OPEN_ERROR          101
#define IDS_FILEDLG_TITLE            102
#define IDS_MISSING_PRINT_FILENAME   103
#define IDS_INVALID_KIND_OPTION      104

#define IDS_REVERTMESSAGE            113
#define IDS_REVERTBOX_TITLE          114

//drafts
#define DRAFT_HELP                  105
#define DID_CREATE                  106
#define DRAFT_TEXT_ID               107
#define DRAFT_NAME                  108
#define DRAFT_COMMENTS              109
#define DRAFT_CANCEL                300

#define DOC_REVERT                  112
#define DRAFT_DEL                   110
#define ID_DLG_DRAFT_CREATE         111

#define ID_NOTEBOOK                 115
#define ID_NB_DRAFTNUM              120
#define ID_NB_DRAFTS                121
#define ID_NB_DRAFT_TITLE           122
#define ID_NB_DRAFT_CREATOR         123
#define ID_NB_DRAFT_CR_TIME         124
#define ID_NB_DRAFT_CR_DATE         125
#define ID_NB_DRAFT_COMMENTS        126

#define WM_CORRECTSPLITBAR          WM_USER+10

// Defines for Hard-coded values for Unicode Implementation
#define DOC_OPENSHELL               201
#define DOC_UNTITLEDDOC             202
#define DOC_OPENSHELLFRAME          203
#define DOC_UNTITLED                204
#define DOC_NODEFQUE                205
#define DOC_ERRPRTQUE               206
#define DOC_TEMPFILE                207
#define DOC_TEMPFOLDER              208
#define DOC_WHERESAVEFILE           209
#define DOC_SAVELETTER              210
#define DOC_NATIVEFORMAT            211
#define DOC_RLSMENUEVENT            212
#define DOC_RLSICONEVENT            213
#define DOC_RLSTREEEVENT            214
#define DOC_FAKEMAILER              215
#define DOC_OPENDOC                 216
#define DOC_OPENDOCSYSTEM           217
#define DOC_PARTKINDNOTREG          218
#define DOC_NOROOTPARTKIND          219
#define DOC_OPENDOCEXCEPT           220
#define DOC_OUTOFMEMORY             221
#define DOC_ERROR                   222
#define DOC_ERROR1                  223
#define DOC_VERSION                 224
#define DOC_PRINT2                  225
#define DOC_SHOW                    226
#define DOC_SAVEFILE                227
#define DOC_DIR                     228
#define DOC_DRAGGING                229
#define DOC_ABOUTOPENDOC            230
#define DOC_SHELLTEMPFILE           231
#define DOC_OPENDOCDOCUMENT         232
#define DOC_NOPARTEDITOR            233
#define DOC_COPY                    234
#define DOC_EXCEPTION               236
#define DOC_STATIONARY              237
#define DOC_TEMPFILE2               238
#define DOC_DRAFTWINDOW             239
#define DOC_DFAFTWINDOW1            240
#define DOC_DRAFTS2                 241
#define DOC_DOCSHELL                242
#define DOC_DRAFTPROPS              243
#define DOC_DRAFTWINPROPS           244
#define DOC_DRAFTLISTDOC            245
#define DOC_UNKNOWN                 246
#define DOC_NOCOMMENTS              247
#define DOC_DISPOSEDESC             248
#define DOC_ODGETTEXTPROP           249
#define DOC_ODSETTEXTPROP           250
#define DOC_SEMUNTITLED             251
#define DOC_CANNOTOPENSEM           252
#define DOC_CANNOTGETSEM            253
