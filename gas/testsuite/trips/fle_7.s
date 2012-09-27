; Test:     fle_7
; Author:   Karu
; Purpose:  Test generates 2.5 and 1.5 and compares
;           comparve +ve, +ve
; Final result should be 0x0

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] genu 0x4004 N[1]
N[1] app 0x0 N[2]
N[2] app 0x0 N[3]
N[3] app 0x0 N[8,0]
N[4] genu 0x3ff8 N[5]
N[5] app 0x0 N[6]
N[6] app 0x0 N[7]
N[7] app 0x0 N[8,1]
N[8] fle W[3]
N[9] bro exit
       
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
