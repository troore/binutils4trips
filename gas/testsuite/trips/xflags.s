; Test:     xflags
; Author:   Robert McDonald/Yoder
; Purpose:  Tests xflags in block header instruction
; 
; Use '% tas -D xflags.s | less' to check header words 26 and 27.

; Final result should be 0x24.
;
; The math is as follows:
;           result = (6 + MATH_VALUE * 3)
;
; $Id: xflags.s,v 1.1 2005/05/09 14:28:57 byoder Exp $

.set SYS_EXIT=1
.set INHIBIT_BRPRED = 1
.set INHIBIT_LDPRED = 2
.set BLOCK_SYNC     = 4
.set XFLAGS=(INHIBIT_BRPRED | INHIBIT_LDPRED | BLOCK_SYNC)

.set MATH_VALUE=10

.text
.global _start

.bbegin _start XFLAGS
; .bbegin _start 300
; .bbegin _start (0x4 + 0x6)

N[0] movi MATH_VALUE N[1]
N[1] mov3 N[8,0] N[9,0] N[10,0]
N[2] movi 2 N[3]
N[3] mov3 N[8,1] N[9,1] N[10,1]
N[4] movi 1 N[5]
N[5] mov3 N[8,P] N[9,P] N[10,P]

N[8] add_t N[11,0]
N[9] add_t N[11,1]
N[10] add_t N[12,0]
N[11] add N[12,1]
N[12] add N[13]

N[13] mov3 W[5] W[4] W[3]
W[3] write G[3]
W[4] write G[4]
W[5] write G[5]

N[127] bro exit
.bend

.bbegin exit	      
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
