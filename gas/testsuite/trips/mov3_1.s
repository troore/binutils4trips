; Test:     mov3_1
; Author:   Robert McDonald
; Purpose:  Tests mov3 instruction

; Final result should be 0x0f

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] movi 3 N[1]
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
