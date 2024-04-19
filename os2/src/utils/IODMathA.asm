; @(#) 1.10 os2/src/utils/IODMathA.asm, odutils, od96os2, odos29646d 10/31/96 10:18:00 [11/15/96 15:49:43]
;====START_GENERATED_PROLOG======================================
;
;
;   COMPONENT_NAME: odutils
;
;   CLASSES: none
;
;   ORIGINS: 82,27
;
;
;   (C) COPYRIGHT International Business Machines Corp. 1995,1996
;   All Rights Reserved
;   Licensed Materials - Property of IBM
;   US Government Users Restricted Rights - Use, duplication or
;   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
;   	
;   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
;   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
;   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
;   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
;   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
;   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
;   OR PERFORMANCE OF THIS SOFTWARE.
;
;====END_GENERATED_PROLOG========================================
;

;*******************************************************************
;*
;* Copyright (C) Apple Computer, Inc., 1994
;*
;*******************************************************************
        TITLE   ODMATHA.ASM
        .486
        .model FLAT

        .code

ODWide STRUC
highdd DD      ?   ; high double word
lowdd  DD      ?   ; low  double word
ODWide ENDS

;******** From ODMath.h *************
kODFixedInfinity       equ 07FFFFFFFh     ; ced [117913]
kODFixedMinusInfinity  equ 080000000h     ; ced [117913]
kODIgnoreRemainder     equ 0FFFFFFFFh     ; ced [117913]


USE_SYSTEM_CALLING_CONVENTION equ 0

IF USE_SYSTEM_CALLING_CONVENTION
arg1  EQU  <[ESP+4]>
arg2  EQU  <[ESP+8]>
arg3  EQU  <[ESP+12]>
ENDIF

;*********************************************************************
;
;  ODWideMultiply -
;
;  C language prototype
;     ODWide*  ODWideMultiply( ODSLong a, ODSLong b, ODWide *result )
;
;  Multiplies a and b and places product in result.
;  Returns result ponter
;
;  Input parameters using C/Set++ Optlink calling convention:
;     EAX = a
;     EDX = b
;     ECX = result pointer
;
;  On return:
;     EAX = result pointer
;
;*********************************************************************
        ALIGN 04H
IFDEF _PLATFORM_WIN32_
        PUBLIC ?ODWideMultiply
?ODWideMultiply  PROC
ELSE
        PUBLIC ODWideMultiply
ODWideMultiply  PROC
ENDIF

IF USE_SYSTEM_CALLING_CONVENTION
   MOV   EAX,arg1
   MOV   EDX,arg2
   MOV   ECX,arg3
ENDIF
   IMUL  EDX
   MOV   (ODWide PTR [ECX]).lowdd,EAX
   MOV   (ODWide PTR [ECX]).highdd,EDX
   MOV   EAX,ECX
   RET
IFDEF _PLATFORM_WIN32_
?ODWideMultiply  ENDP
ELSE
ODWideMultiply  ENDP
ENDIF

;*********************************************************************
;
;  ODWideDivide -
;
;  C language prototype
;     ODSLong  ODWideDivide( const ODWide *dividend,
;                               ODSLong divisor, ODSLong *remainder)
;
;  Divides dividend by divisor and returns quotient and places
;  remainder in 'remainder'.
;
;  Returns quotient
;
;  Input parameters using C/Set++ Optlink calling convention:
;     EAX = dividend pointer
;     EDX = divisor
;     ECX = remainder pointer
;
;  On return:
;     EAX = quotient
;
;*********************************************************************
        ALIGN 04H
IFDEF _PLATFORM_WIN32_
        PUBLIC ?ODWideDivide
?ODWideDivide    PROC
ELSE
        PUBLIC ODWideDivide
ODWideDivide    PROC
ENDIF

IF USE_SYSTEM_CALLING_CONVENTION
   MOV   EAX,arg1
   MOV   EDX,arg2
   MOV   ECX,arg3
ENDIF
   ; If performing the signed division would result in a divide by
   ; zero exception, we want to detect that and return
   ; kODMinusInfinity instead.  A divide by zero exception for
   ; signed division will result if the absolute value of the dividend
   ; divided by 2*31 is greater than the absolute value of the divisor.

;  [120333] start  - ced
   PUSH  EAX                 ; save gp registers
   PUSH  EBX
   PUSH  EDX

   MOV   EBX,(ODWide PTR [EAX]).lowdd     ;Get abs value of dividend in EAX:EBX
   MOV   EAX,(ODWide PTR [EAX]).highdd
;  .if   <EAX lt 0>
   TEST  EAX,EAX
	JGE   label1
      CLC
      NOT   EBX
      NOT   EAX
      ADD   EBX,1
      ADC   EAX,0
;  .endif
label1:
;  .if   <EDX lt 0>          ;Get abs value of divisor in EDX
	CMP   EDX,0
	JGE   label2
      NEG  EDX
;  .endif
label2:
   ; Test EAX msb.  If set then dividend was largest neg number in which
   ; case an exception would result no matter what value the divisor is.
   SAL   EAX,1
;  .if   <nc>                ;greatest neg number is special case
	JC    label3
      SHR   EAX,1            ;Restore EAX
      SHRD  EBX,EAX,31       ;Divide dividend by 2**31
      CMP   EDX,EBX          ;Divisor must be gt dividend / (2**31)
;  .endif
label3:
   POP   EDX
   POP   EBX
   POP   EAX

   ; Check result of comparison.  Remember, we're using unsigned numbers
   ; so the carry flag is used to indicate that the source operand is
   ; greater than the destination operand rather than the sign flag.

;  .if   <c> or
;  .if   <eq>
   JA    label4
      MOV  EAX,kODFixedMinusInfinity
      RET
;  .endif
label4:
;  [120333] end  - ced

   ; Looks like we're OK.  Go ahead and do the division now.

   PUSH  ECX
   MOV   ECX,EDX
   MOV   EDX,(ODWide PTR [EAX]).highdd
   MOV   EAX,(ODWide PTR [EAX]).lowdd
   IDIV  ECX
   POP   ECX
;  .if   <ecx ne kODIgnoreRemainder> and   ; ced [117913]
   CMP   ECX,kODIgnoreRemainder
	JE    return
;  .if   <ecx ne 0>                        ; ced [117913]
   TEST  ECX,ECX
	JZ    return
      MOV   [ECX],EDX                      ; ced [117913]
;  .endif
return:
   RET
IFDEF _PLATFORM_WIN32_
?ODWideDivide   ENDP
ELSE
ODWideDivide   ENDP
ENDIF

END

