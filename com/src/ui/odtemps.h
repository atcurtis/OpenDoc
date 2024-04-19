/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odui
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
/* @(#) 1.3 com/src/ui/odtemps.h, odui, od96os2, odos29646d 10/31/96 12:46:09 [11/15/96 16:00:27] */
/* Header file for C APIs for Create and Remove template. */
/*
 *   Change History (most recent first):
 *
 *     143601   11/13/95   pfe      template calls cleanup
 *     127295   06/27/95   pfe      change to ODCreateTemplate call
 */

#ifndef _ODTEMPS_H
#define _ODTEMPS_H

#ifdef __cplusplus
extern "C" {
#endif
typedef char* ODType;

PSZ APIENTRY ODCreateTemplate(ODType partKindName,ODEditor partEditor); //127295 [pfe]

ODBoolean APIENTRY ODDeleteTemplate(PSZ ObjectID);

#ifdef __cplusplus
};
#endif

#endif
