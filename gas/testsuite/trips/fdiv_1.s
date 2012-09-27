; Test:     fdiv_1
; Author:   Karu
; Purpose:  Tests fdiv instruction, div 27 by 4.0, result should be 6.75
;           These are NOT rigorous tests at all. They focus more on
;           checking if instruction decode is right.

; Final result should be 0x401b000000000000
; 6.75

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] gens 27 N[2,0]
N[1] gens 1 N[3,0]
N[2] fitod N[4,0]
N[3] fitod N[4,1]
N[4] fdiv N[10,0]        
N[5] gens 4 N[7,0]
N[6] gens 1 N[8,0]
N[7] fitod N[9,0]
N[8] fitod N[9,1]
N[9] fdiv N[10,1]        
N[10] fdiv W[3]
N[11] bro exit
       
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
