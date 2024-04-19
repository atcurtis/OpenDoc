/* @(#)Z 1.4 com/src/registry/csetwrap.h, odconfig, od96os2, odos29646d 96/11/15 15:49:28 (96/10/29 09:29:37) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odconfig
//
//   CLASSES:   CWrap
//		FileExtensionSet
//		FileTypeSet
//		KindSet
//		PartSet
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

#ifndef CSETWRAP_HPP
#define CSETWRAP_HPP

#include <som.xh>
#include <iset.h>
#include <ODTypes.h>
#include <PartInfi.xh>
#include <PartKind.xh>
#include <PartCat.xh>

/*********************************************************************/
/* Wrapper for SOM class because ISet expects elements to overload   */
/* operators.                                                        */
/*********************************************************************/
template <class Element>
class CWrap
{ 
    const Element* e1;
public:

    CWrap( const CWrap<Element>& cw) : e1( cw)
    {
    }
    CWrap( Element* e1) : e1( e1) 
    {
    }
    ~CWrap()
    {
    }
    operator const Element* () const
    {
        return e1;
    }
    operator Element* ()
    {
        return (Element*)e1;
    }
    CWrap<Element>& operator =        (CWrap<Element> const& cw)
    {
        e1 = cw;
        return *this;
    }
    Boolean operator ==        (CWrap<Element> const& cw) const
    {
        return !compare( cw);
    }
    Boolean operator >         (CWrap<Element> const& cw) const
    {
        return compare( cw) > 0;
    }
    Boolean operator <         (CWrap<Element> const& cw) const
    {
        return compare( cw) < 0;
    }
    long          compare      (const Element* e2) const
    {
        return strcmp( ((Element*)e1)->GetKey( somGetGlobalEnvironment()), 
                       ((Element*)e2)->GetKey( somGetGlobalEnvironment()) );
    }
};
 
/*********************************************************************/
/* Wrapper for template, because SOM "no like templates."            */
/*********************************************************************/
class PartSet : public ISet< CWrap<ODPartHandlerInfoInt> >
{
};

/*********************************************************************/
/* Wrapper for template, because SOM "no like templates."            */
/*********************************************************************/
class KindSet : public ISet< CWrap<ODPartKind> >
{
};

/*********************************************************************/
/* Wrapper for template, because SOM "no like templates."            */
/*********************************************************************/
class FileTypeSet : public ISet< CWrap<ODPartFileType > >
{
};

/*********************************************************************/
/* Wrapper for template, because SOM "no like templates."            */
/*********************************************************************/
class FileExtensionSet : public ISet< CWrap<ODPartFileExtension > >
{
};
#endif
