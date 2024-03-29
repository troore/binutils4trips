; Test:	rt_dep_1
; Author:	Ramadass Nagarajan
; Purpose:	A loop with null writes and dependences that span 1 block(s).
; 		Checks if register forwarding across blocks works correctly.
; Note:		Test generated by a script
;
; Final result should be 0x4e8



.set SYS_EXIT=1

.data

;; define memory locations for some variables
index:	
	.quad 0x10
num:	
	.quad 0x100

.text
.global _start

;;  set loop count to 50, clear index, and initialize all registers
.bbegin _start

N[0] movi 50 N[3,1]
N[1] genu %lo(num) N[2]
N[2] app %bottom(num) N[3,0]
N[3] sw	0

N[4] movi 0 N[7,1]
N[5] genu %lo(index) N[6]
N[6] app %bottom(index) N[7,0]
N[7] sw	0

N[8] bro body1_p

N[9] movi 0 W[0]
N[10] movi 1 W[4]
N[11] movi 2 W[8]
N[12] movi 3 W[12]
N[13] movi 4 W[16]
N[14] movi 5 W[20]
N[15] movi 6 W[24]
N[16] movi 7 W[28]

W[0] write G[0]
W[4] write G[4]
W[8] write G[8]
W[12] write G[12]
W[16] write G[16]
W[20] write G[20]
W[24] write G[24]
W[28] write G[28]
.bend


;; read 8 registers. add 1 to each and write them back.
;; write the same values onto another register set as well.
;; do all of this only in alternate iterations
.bbegin body1_p
R[0] read G[0] N[0,0] N[64,0]
R[4] read G[4] N[1,0] N[65,0]
R[8] read G[8] N[2,0] N[66,0]
R[12] read G[12] N[3,0] N[67,0]
R[16] read G[16] N[4,0] N[68,0]
R[20] read G[20] N[5,0] N[69,0]
R[24] read G[24] N[6,0] N[70,0]
R[28] read G[28] N[7,0] N[71,0]


;; read index
N[16] genu %lo(index) N[17]
N[17] app %bottom(index) N[18]
N[18] mov N[19]
N[19] lw 0 N[20]

N[20] andi 1 N[21]
N[21] teqi 0 N[22]
N[22] mov N[0,P] N[23]
N[23] mov N[1,P] N[24]
N[24] mov N[2,P] N[25]
N[25] mov N[3,P] N[26]
N[26] mov N[4,P] N[27]
N[27] mov N[5,P] N[28]
N[28] mov N[6,P] N[29]
N[29] mov N[7,P] N[30]
N[30] mov N[64,P] N[31]
N[31] mov N[65,P] N[32]
N[32] mov N[66,P] N[33]
N[33] mov N[67,P] N[34]
N[34] mov N[68,P] N[35]
N[35] mov N[69,P] N[36]
N[36] mov N[70,P] N[37]

N[37] mov N[71,P] N[38]
N[38] mov N[48,P] N[39]
N[39] mov N[49,P] N[40]
N[40] mov N[50,P] N[41]
N[41] mov N[51,P] N[42]
N[42] mov N[52,P] N[43]
N[43] mov N[53,P] N[44]
N[44] mov N[54,P] N[45]
N[45] mov N[55,P] N[46]
N[46] mov N[96,P] N[47]
N[47] mov N[97,P] N[72]
N[72] mov N[98,P] N[73]
N[73] mov N[99,P] N[74]
N[74] mov N[100,P] N[75]        
N[75] mov N[101,P] N[76]
N[76] mov N[102,P] N[103,P]
    
N[0] addi_t 1 W[0]
N[48] null_f W[0]
N[64] addi_t 1 W[3]
N[96] null_f W[3]
N[1] addi_t 1 W[4]
N[49] null_f W[4]
N[65] addi_t 1 W[7]
N[97] null_f W[7]
N[2] addi_t 1 W[8]
N[50] null_f W[8]
N[66] addi_t 1 W[11]
N[98] null_f W[11]
N[3] addi_t 1 W[12]
N[51] null_f W[12]
N[67] addi_t 1 W[15]
N[99] null_f W[15]
N[4] addi_t 1 W[16]
N[52] null_f W[16]
N[68] addi_t 1 W[19]
N[100] null_f W[19]
N[5] addi_t 1 W[20]
N[53] null_f W[20]
N[69] addi_t 1 W[23]
N[101] null_f W[23]
N[6] addi_t 1 W[24]
N[54] null_f W[24]
N[70] addi_t 1 W[27]
N[102] null_f W[27]
N[7] addi_t 1 W[28]
N[55] null_f W[28]
N[71] addi_t 1 W[31]
N[103] null_f W[31]

N[127] bro body1

W[0] write G[0]
W[3] write G[3]
W[4] write G[4]
W[7] write G[7]
W[8] write G[8]
W[11] write G[11]
W[12] write G[12]
W[15] write G[15]
W[16] write G[16]
W[19] write G[19]
W[20] write G[20]
W[23] write G[23]
W[24] write G[24]
W[27] write G[27]
W[28] write G[28]
W[31] write G[31]

.bend


;; read 8 registers. add 1 to each and write them back.
;; write the same values onto another register set as well. 
.bbegin body1
R[0] read G[0] N[0,0] N[64,0]
R[4] read G[4] N[1,0] N[65,0]
R[8] read G[8] N[2,0] N[66,0]
R[12] read G[12] N[3,0] N[67,0]
R[16] read G[16] N[4,0] N[68,0]
R[20] read G[20] N[5,0] N[69,0]
R[24] read G[24] N[6,0] N[70,0]
R[28] read G[28] N[7,0] N[71,0]

N[0] addi 1 W[0]
N[64] addi 1 W[3]
N[1] addi 1 W[4]
N[65] addi 1 W[7]
N[2] addi 1 W[8]
N[66] addi 1 W[11]
N[3] addi 1 W[12]
N[67] addi 1 W[15]
N[4] addi 1 W[16]
N[68] addi 1 W[19]
N[5] addi 1 W[20]
N[69] addi 1 W[23]
N[6] addi 1 W[24]
N[70] addi 1 W[27]
N[7] addi 1 W[28]
N[71] addi 1 W[31]

W[0] write G[0]
W[3] write G[3]
W[4] write G[4]
W[7] write G[7]
W[8] write G[8]
W[11] write G[11]
W[12] write G[12]
W[15] write G[15]
W[16] write G[16]
W[19] write G[19]
W[20] write G[20]
W[23] write G[23]
W[24] write G[24]
W[27] write G[27]
W[28] write G[28]
W[31] write G[31]



;;  read loop count
N[10] genu %lo(num) N[11]
N[11] app %bottom(num) N[12]
N[12] lw 0 N[20,0]
    
;; read index
N[14] genu %lo(index) N[15]
N[15] app %bottom(index) N[16]
N[16] mov N[17] N[23,0]
N[17] lw 0 N[18]	
    
;;  add 1 to loop index
N[18] addi 1 N[19]
N[19] mov N[20,1] N[23,1]
    
;;  test if loop count is reached
N[20] teq N[21,P] N[22,P]
    
N[21] bro_f body1_p
N[22] bro_t exit
    
;;  write loop index
N[23] sw 0
    
.bend

; exit block. add all registers written and write the sum to G[4]
.bbegin exit

R[0] read G[0] N[0]
R[3] read G[3] N[64]
R[4] read G[4] N[1]
R[7] read G[7] N[65]
R[8] read G[8] N[2]
R[11] read G[11] N[66]
R[12] read G[12] N[3]
R[15] read G[15] N[67]
R[16] read G[16] N[4]
R[19] read G[19] N[68]
R[20] read G[20] N[5]
R[23] read G[23] N[69]
R[24] read G[24] N[6]
R[27] read G[27] N[70]
R[28] read G[28] N[7]
R[31] read G[31] N[71]

N[0] addi 0 N[1,1]
N[1] add N[2,1]
N[2] add N[3,1]
N[3] add N[4,1]
N[4] add N[5,1]
N[5] add N[6,1]
N[6] add N[7,1]
N[7] add N[64,1]

N[64] add N[65,1]
N[65] add N[66,1]
N[66] add N[67,1]
N[67] add N[68,1]
N[68] add N[69,1]
N[69] add N[70,1]
N[70] add N[71,1]
N[71] add W[3]										 							
N[8] movi SYS_EXIT W[0]
N[9] scall
    
W[0] write G[0]
W[3] write G[3]
    
.bend

