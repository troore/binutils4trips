; Test:     mov4_1
; Author:   Robert McDonald
; Purpose:  Tests mov4 instruction

; Final result should be 0x14

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] movi 3 N[1]
N[1] mov4 N[8,0] N[9,0] N[10,0] N[11,0]
N[2] movi 2 N[3]
N[3] mov4 N[8,1] N[9,1] N[10,1] N[11,1]
N[4] movi 1 N[5]
N[5] mov4 N[8,P] N[9,P] N[10,P] N[11,P]

N[8] add_t N[12,0]
N[9] add_t N[12,1]
N[10] add_t N[13,0]
N[11] add_t N[13,1]

N[12] add N[14,0]
N[13] add N[14,1]
N[14] add N[20]

N[20] mov4 W[5] W[4] W[3] W[6]
W[3] write G[3]
W[4] write G[4]
W[5] write G[5]
W[6] write G[6]

N[127] bro exit
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
