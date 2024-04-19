/* @(#)Z 1.3 os2/src/registry/odreggui/prefedit.h, odconfig, od96os2, odos29646d 96/11/15 15:52:48 (96/08/23 01:42:44) */
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

// *********************************************************************
//
//  File:      prefedit.h
//  Contains:  Registry GUI header
//
// ************************************************************************

#define    ID_DIALOG    100
#define    IDM_DIALOG   101
#define    IDD_HELP     102
#define    IDD_KINDBOX  103
#define    IDD_PARTBOX  104
#define    IDM_EXIT     105
#define    IDD_OK       106
#define    KIND_CAPTION                107
#define    EDITOR_CAPTION              108
#define    IDD_ICON                    109


#define WARN_TITLE "WARNING"
#define WARN_MSG   "Preferred Editor Set.\n\
If any documents are currently open, the changes won't take\n\
effect until current documents are closed and reopened.\n"
#define DIALOG_TITLE "Preferred Editor Selection"
#define PART_FAIL "Unable to load Part Editor List, return code = %u\n"
#define KIND_FAIL "Unable to load Part Kind List, return code =  %u\n"
#define REG_FAIL  "Unable to set Preferred Editor, return code =  %u\n"
#define HELP_MSG "Display a list of part editors by\n\
double clicking on a part kind. \n\
\n\
Choose a part editor by clicking on it,\n\
the default editor will be highlighted.\n\
\n\
Set the preferred part editor by\n\
clicking on the OK button.\n"
#define RC_NOT_OPEN "prefedit: Could not load odprefed.dll.\n"
