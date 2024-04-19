/* @(#)Z 1.6 com/src/registry/rmutil.h, odconfig, od96os2, odos29646d 96/11/15 15:49:29 (96/10/29 09:30:22) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odconfig
 *
 *   CLASSES: none
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

#ifndef _RMUTIL_
#define _RMUTIL_

#ifdef _PLATFORM_OS2_ // aml - [140007]
#ifndef _STDDEFS_
#include <StdDefs.xh>
#endif
#endif

inline string newString( string iString)
{
    if ( iString )
    {
        string s = (string )SOMMalloc( strlen(iString)+1);
        return strcpy( s, iString);
    }
    return iString;
}

/*********************************************************************/
/* Some useful macros                                                */
/*********************************************************************/

#define deleteStringSequence( seq)                                 \
    { /* New block to avoid var name collision */                  \
        for ( long i = 0; i < seq. _length; i++)                   \
        {                                                          \
            SOMFree( seq. _buffer[i]);                             \
        }                                                          \
        SOMFree( seq. _buffer);                                    \
    }

#define showSeq( seq, level)                                       \
    somLPrintf( level, "%s\n", #seq);                              \
    somLPrintf( (level)+1, "_maximum = %d\n",  (seq). _maximum);   \
    somLPrintf( (level)+1, "_length  = %d\n",  (seq). _length);    \
    somLPrintf( (level)+1, "_buffer  = %0x\n", (seq). _buffer)

#define CP { somLPrintf( 10, "Line %d in %s\n", __LINE__, __FILE__);}

#define SSTRLEN(s) s?strlen(s):0

#define SSTRCAT(t, s) s?strcat(t, s):strcat(t, "");

#if 0
#define STRUCTCAT( ptr, strct, elem, src) \
        {if(src){strct->elem=ptr+strlen(ptr);strcat(ptr,src);}else{strct->elem=0;}}
#else
#define STRUCTCAT( ptr, strct, elem, src) \
        {if(src){strct->elem=ptr;strcat(ptr,src);ptr=strct->elem+strlen(strct->elem)+1;} \
        else{strct->elem=0;}}
#endif

#define BUILDRXSTRING(t, s) { \
  strcpy((t)->strptr,(s));\
  (t)->strlength = strlen((s)); \
}

#define BENTOSEMNAME "\\SEM32\\BENTOSEM"

#if 0  // defect 22535 - these symbols are currently not used in the code
#ifdef _PLATFORM_OS2_  // aml - [140007] - display names for standard OS2 kinds ( clipboard and drag-drop formats)

struct StdOS2Kind {
   ODISOStr kindName;
   ODISOStr dispName;
};

static const StdOS2Kind OS2Kinds[] = {
{ kODKindOS2Bitmap,       kOS2BitmapDisplayName       },
{ kODKindOS2DspBitmap,    kOS2DspBitmapDisplayName    },
{ kODKindOS2Metafile,     kOS2MetafileDisplayName     },
{ kODKindOS2DspMetafile,  kOS2DspMetafileDisplayName  },
{ kODKindOS2Text,         kOS2TextDisplayName         },
{ kODKindOS2DspText,      kOS2DspTextDisplayName      },
{ kODKindOS2DIB,          kOS2DIBDisplayName          },
{ kODKindOS2DIF,          kOS2DIFDisplayName          },
{ kODKindOS2OEMText,      kOS2OEMTextDisplayName      },
{ kODKindOS2OwnerDisplay, kOS2OwnerDisplayDisplayName },
{ kODKindOS2PtrPict,      kOS2PtrPictDisplayName      },
{ kODKindOS2RTF,          kOS2RTFDisplayName          },
{ kODKindOS2SYLK,         kOS2SYLKDisplayName         },
{ kODKindOS2TIFF,         kOS2TIFFDisplayName         },
{ kODKindOS2Palette,      kOS2PaletteDisplayName      },
{ kODKindOS2Unknown,      kOS2UnknownDisplayName      }
};

#define NumStdOS2Kinds  (sizeof(OS2Kinds)/sizeof(StdOS2Kind))

inline ODISOStr GetDisplayNameForOS2Kind(ODISOStr kind)
{
   for(int i=0; i<NumStdOS2Kinds; i++) {
      if(!strcmp(OS2Kinds[i].kindName,kind))
        return OS2Kinds[i].dispName;
   }
   return kODNULL;
}
#endif // _PLATFORM_OS2_
#endif // symbols taken out - defect 22535

#endif
