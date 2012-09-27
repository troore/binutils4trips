; Test:     fdtos_1
; Author:   Karu
; Purpose:  Test generates all 1.5 and converts to single

; Final result should be 0x3fc00000
; -1.5

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] genu 0x3ff8 N[1]
N[1] app 0x0 N[2]
N[2] app 0x0 N[3]
N[3] app 0x0 N[4]     
N[4] fdtos W[3] 
N[5] bro exit
       
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
