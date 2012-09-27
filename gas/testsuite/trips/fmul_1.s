; Test:     fmul_1
; Author:   Karu
; Purpose:  Tests fmul instruction, multiply 1.5 from 2.7, result should be 4.05

; Final result should be 0x4010333333333334
; 4.05

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] genu 0x3ff8 N[1]
N[1] app 0x0 N[2]
N[2] app 0x0 N[3]
N[3] app 0x0 N[10]              ;  1.5

N[5] genu 0x4005 N[6]
N[6] app 0x9999 N[7]
N[7] app 0x9999 N[8]
N[8] app 0x999a N[11]           ;  2.7

N[10] mov N[12]
N[11] mov N[12,1]
        
N[12] fmul W[3]
N[127] bro exit
        
W[3] write G[3]
                                        

.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
