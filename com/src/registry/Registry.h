/* @(#)Z 1.4 com/src/registry/Registry.h, odconfig, od96os2, odos29646d 96/11/15 15:49:28 (96/10/29 09:29:32) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odconfig
 *
 *   CLASSES:   Node
 *		Path
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

#ifndef REGISTRY_HPP
#define REGISTRY_HPP

//#include <ctype.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include    <rstypes.h>
#include    <name.h>
#include    <unicode.h>


class Node;
class Path;

// ***************************************************************************
// *  CLASS:        Path
// * 
// *  DESCRIPTION:  Wraps naming services APIs and structures for link names 
// *                with a convenient, easy to use class.
// * 
// * 
// * 
// ***************************************************************************

class Path
{
public:
    Path( unsigned short count, char* name = 0);
    long setLinkName( unsigned short index, char* name);
    operator ns_path_t () { return path; }

private:
    unsigned short count;
    ns_path_t path;
};

// ***************************************************************************
// *  CLASS:        Node
// * 
// *  DESCRIPTION:  Wraps naming services APIs and structures for node with a
// *                convenient, easy to use class.
// * 
// * 
// * 
// ***************************************************************************

class Node
{
public:
    static Node& getRootNode();

    Node( ns_node_handle_t nodeHandle) { handle = nodeHandle; }
    Node( unsigned short attrCount, 
          ns_node_type_t nodeType,
          unsigned short nodeFlags = 0);

    ns_error_t addNode( Path& path, Node* newNode, int flags = 0);
    ns_error_t resolve( Path& path, Node** newNode, int flags = 0);

    long setAttribute( unsigned short index, const char* name, 
                       const char* value, unsigned short userType = 0 );

    long setAttribute( unsigned short index, const char* name, 
                       const short value, unsigned short userType = 0 );
    long setAttribute( unsigned short index, const char* name, 
                       const unsigned short value, unsigned short userType = 0 );

    long setAttribute( unsigned short index, const char* name, 
                       const long value, unsigned short userType = 0 );
    long setAttribute( unsigned short index, const char* name, 
                       const unsigned long value, unsigned short userType = 0 );

    long setAttribute( unsigned short index, 
                       const char* name,
                       const ns_typed_value_t& value);

private:  //## member data ##//
    ns_node_t content;
    ns_node_handle_t handle;

private: //## member functions ##//
    ns_attr_t* getAttr( unsigned short index)
    {
        if ( index < content. count)
        {
            return &content. attrs[index];
        }
        return 0;
    }

};
#endif
