; Test:		pred_add_1
; Author:	Ramadass Nagarajan
; Purpose:	Tests if predicated adds work correctly
;
; Final result should be 0x3

.set SYS_EXIT=1

.data
index:	
	.quad 0x10

.text
.global _start


;;  set index to 0
.bbegin _start

N[4] movi 0 N[7,1]
N[5] genu %lo(index) N[6]
N[6] app %bottom(index) N[7,0]
N[7] sw	0

N[8] bro init0
.bend

;;  clear all registers
.bbegin init0

N[0] movi 0 W[0]
N[1] movi 1 W[1]
N[2] movi 2 W[2]
N[3] movi 3 W[3]
N[4] movi 4 W[4]
N[5] movi 5 W[5]
N[6] movi 6 W[6]
N[7] movi 7 W[7]

N[127] bro block0

W[0] write G[0]
W[1] write G[1]
W[2] write G[2]
W[3] write G[3]
W[4] write G[4]
W[5] write G[5]
W[6] write G[6]
W[7] write G[7]

.bend

;; read 8 registers
.bbegin block0

R[0] read G[0] N[0,0]
R[1] read G[1] N[1,0]
R[2] read G[2] N[2,0]
R[3] read G[3] N[3,0]
R[4] read G[4] N[0,1]
R[5] read G[5] N[1,1]
R[6] read G[6] N[2,1]
R[7] read G[7] N[3,1]

;; read index
N[32] genu %lo(index) N[33]
N[33] app %bottom(index) N[34]
N[34] lw 0 N[35]

;;  generate and distribute predicate
N[35] andi 1 N[36]
N[36] mov N[0,P] N[37]    
N[37] mov N[1,P] N[38]
N[38] mov N[2,P] N[39]    
N[39] mov N[3,P] N[52]
N[52] mov N[16,P] N[53]    
N[53] mov N[17,P] N[54]
N[54] mov N[18,P] N[55]    
N[55] mov N[19,P]
		
N[0] add_t W[0]
N[16] null_f W[0]
N[1] add_t W[1]
N[17] null_f W[1]
N[2] add_t W[2]
N[18] null_f W[2]
N[3] add_t W[3]
N[19] null_f W[3]

N[127] bro exit

W[0] write G[0]
W[1] write G[1]
W[2] write G[2]
W[3] write G[3]

.bend
	
.bbegin exit

N[8] movi SYS_EXIT W[0]
N[9] scall
    
W[0] write G[0]
    
.bend
