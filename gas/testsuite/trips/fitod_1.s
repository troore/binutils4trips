; Test:     fitod_1
; Author:   Karu
; Purpose:  Tests generates 1 and converts to double 1

; Final result should be 1

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] gens 1 N[1]
N[1] fitod W[3]
N[2] bro exit
       
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
