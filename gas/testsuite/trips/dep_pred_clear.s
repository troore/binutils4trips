; Test:    dep_pred_1
; Author:  Raj
; Purpose: Test 1-bit and 2-bit clearing dependence predictors 
        ;; The load_block has two loads - one of them has an all-store
        ;; dependence while the other has one-store dependence. This load
        ;; benefits from the 2-bit predictor.
; Final result should be 0x1024

.set SYS_EXIT=1

.data
testdata:
.fill 16384, 1, 0
 
.text

                
.global _start
.bbegin _start
N[0] genu 0x1024 W[1]
N[1] movi 0 W[4]
N[2] movi 0 W[3]
N[3] movi 0 W[6]        
N[4] bro load_block

W[1] write G[1]
W[3] write G[3]        
W[4] write G[4]
W[6] write G[6]        
.bend
              
.bbegin load_block
R[1] read G[1] N[37,1]
R[4] read G[4] N[36]
R[3] read G[3] N[35]
R[6] read G[6] N[42] N[43]                
N[4] genu %lo(testdata) N[5]
N[5] app %bottom(testdata) N[42,1]
N[42] add N[6]
N[43] addi 16 W[6]                
N[6] mov N[33] N[7]
N[7] mov N[34] N[8]
N[8] mov N[22] N[9]
N[9] mov N[25] N[10]
N[10] mov N[26] N[11]
N[11] mov N[27] N[12]
N[12] mov N[28] N[13]
N[13] mov N[29] N[30]                                                       
N[30] mov N[31] N[32]
N[31] mov N[24] N[23]        

N[14] genu 0x01 N[22,1]
N[15] genu 0x04 N[23,1]
N[16] genu 0x06 N[24,1]
N[17] genu 0x08 N[25,1]
N[18] genu 0x0a N[26,1]
N[19] genu 0x0c N[27,1]
N[20] genu 0x0e N[28,1]
N[21] genu 0x0f N[29,1]
N[40] genu 0x02 N[32,1]

N[22] sd 8
N[23] sb 0 
N[24] sb 1
N[25] sb 2
N[26] sb 3
N[27] sb 4
N[28] sb 5
N[29] sb 6
N[32] sb 7
N[34] ld 0 W[5]
N[33] ld 8 N[35,1]
N[35] add W[3]                        
N[36] addi 1 N[41]
N[41] mov N[37] W[4]        
N[37] teq N[38, P] N[39,P]
N[38] bro_f load_block
N[39] bro_t exit

W[3] write G[3]
W[4] write G[4]
W[5] write G[5]
W[6] write G[6]        
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend




