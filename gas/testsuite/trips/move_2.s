; Test:     move_2
; Author:   Robert McDonald
; Purpose:  Tests reads, writes, and moves

; Final result should be -7

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] movi 7 W[0]
N[1] movi -7 W[1]
N[127] bro foo
W[0] write G[0]
W[1] write G[1]
.bend

.bbegin foo
R[4] read G[0] N[0] N[1]
R[9] read G[1] N[2]
N[0] mov W[0]
N[1] mov W[14]
N[2] mov W[5] W[31]
N[127] bro exit
W[0] write G[0]
W[5] write G[1]
W[14] write G[2]
W[31] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
