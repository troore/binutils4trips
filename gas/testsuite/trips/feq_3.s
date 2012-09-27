; Test:     feq_3
; Author:   Karu
; Purpose:  Test generates 0 and 0 and compares

; Final result should be 0x1

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] genu 0x0 N[1]
N[1] app 0x0 N[2]
N[2] app 0x0 N[3]
N[3] app 0x0 N[8,0]
N[4] genu 0x0 N[5]
N[5] app 0x0 N[6]
N[6] app 0x0 N[7]
N[7] app 0x0 N[8,1]
N[8] feq W[3]
N[9] bro exit
       
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
