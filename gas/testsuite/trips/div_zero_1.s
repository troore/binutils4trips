; Test:     div_zero_1
; Author:   Robert McDonald
; Purpose:  Test divide by zero exception

; Final result should be an Execute Exception

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] movi 100 N[1]
N[1] divsi 0 W[3]
N[127] bro exit
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
