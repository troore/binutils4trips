; Test:     fne_1
; Author:   Karu, Robert
; Purpose:  Test generates all 1.5 and compares

; Final result should be 0x0

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] genu 0x3ff8 N[1]
N[1] app 0x0 N[2]
N[2] app 0x0 N[3]
N[3] app 0x0 N[4]
N[4] mov N[5,0] N[5,1]
N[5] fne W[3]
N[6] bro exit
       
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
