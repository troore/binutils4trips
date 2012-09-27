; Test:     load_store_blitz_0
; Author:   Karu
; Purpose:  16 Stores followed by 16 loads in same block. loads all 
;           dependent on stores. all uniq addresses

; Final result should be 120 or 0x78

.set SYS_EXIT=1

.data
testdata:
.fill 1024, 1, 0
 
.text
.global _start
.bbegin _start

N[4] genu %lo(testdata) N[5]
N[5] app %bottom(testdata) N[6]
N[6] mov N[64] N[7]
N[7] mov N[65] N[8]
N[8] mov N[66] N[9]
N[9] mov N[67] N[10]
N[10] mov N[68] N[11]
N[11] mov N[69] N[12]
N[12] mov N[70] N[13]
N[13] mov N[71] N[14]
N[14] mov N[72] N[15]                                                       
N[15] mov N[73] N[16]
N[16] mov N[74] N[17]
N[17] mov N[75] N[18]
N[18] mov N[76] N[19]
N[19] mov N[77] N[20]
N[20] mov N[78] N[21]
N[21] mov N[79] N[22]
N[22] mov N[80] N[23]
N[23] mov N[81] N[24]
N[24] mov N[82] N[25]                                                       
N[25] mov N[83] N[26]
N[26] mov N[84] N[27]
N[27] mov N[85] N[28]
N[28] mov N[86] N[29]
N[29] mov N[87] N[30]
N[30] mov N[88] N[31]
N[31] mov N[89] N[32]
N[32] mov N[90] N[33]
N[33] mov N[91] N[34]
N[34] mov N[92] N[35]                                                       
N[35] mov N[93] N[36]
N[36] mov N[94] N[37]
N[37] mov N[95]

N[64] sd 0
N[65] sd 8
N[66] sd 16
N[67] sd 24
N[68] sd 32
N[69] sd 40
N[70] sd 48
N[71] sd 56
N[72] sd 64
N[73] sd 72
N[74] sd 80
N[75] sd 88
N[76] sd 96
N[77] sd 104
N[78] sd 112
N[79] sd 120
N[80] ld 0 W[16]
N[81] ld 8 W[17]
N[82] ld 16 W[18]
N[83] ld 24 W[19]
N[84] ld 32 W[20]
N[85] ld 40 W[21]
N[86] ld 48 W[22]
N[87] ld 56 W[23]
N[88] ld 64 W[24]
N[89] ld 72 W[25]
N[90] ld 80 W[26]
N[91] ld 88 W[27]
N[92] ld 96 W[28]
N[93] ld 104 W[29]
N[94] ld 112 W[30]
N[95] ld 120 W[31]
        
N[96] gens 0 N[64,1]
N[97] gens 1 N[65,1]
N[98] gens 2 N[66,1]
N[99] gens 3 N[67,1]
N[100] gens 4 N[68,1]
N[101] gens 5 N[69,1]
N[102] gens 6 N[70,1]
N[103] gens 7 N[71,1]
N[104] gens 8 N[72,1]
N[105] gens 9 N[73,1]
N[106] gens 10 N[74,1]
N[107] gens 11 N[75,1]
N[108] gens 12 N[76,1]
N[109] gens 13 N[77,1]
N[110] gens 14 N[78,1]
N[111] gens 15 N[79,1]
     
                          
N[0] bro sumblock

W[16] write G[16]
W[17] write G[17]
W[18] write G[18]
W[19] write G[19]
W[20] write G[20]
W[21] write G[21]
W[22] write G[22]
W[23] write G[23]
W[24] write G[24]
W[25] write G[25]
W[26] write G[26]
W[27] write G[27]
W[28] write G[28]
W[29] write G[29]
W[30] write G[30]
W[31] write G[31]
        
              
.bend


       
.bend

.bbegin sumblock
     
R[16] read G[16] N[16]
R[17] read G[17] N[17]
R[18] read G[18] N[18]
R[19] read G[19] N[19]
R[20] read G[20] N[20]
R[21] read G[21] N[21]
R[22] read G[22] N[22]
R[23] read G[23] N[23]
R[24] read G[24] N[24]
R[25] read G[25] N[25]
R[26] read G[26] N[26]
R[27] read G[27] N[27]
R[28] read G[28] N[28]
R[29] read G[29] N[29]
R[30] read G[30] N[30]
R[31] read G[31] N[31]

N[16] addi 0 N[17,1]
N[17] add N[18,1]
N[18] add N[19,1]
N[19] add N[20,1]
N[20] add N[21,1]
N[21] add N[22,1]
N[22] add N[23,1]
N[23] add N[24,1]
N[24] add N[25,1]
N[25] add N[26,1]
N[26] add N[27,1]
N[27] add N[28,1]
N[28] add N[29,1]
N[29] add N[30,1]
N[30] add N[31,1]
N[31] add W[3]

N[32] bro exit

W[3] write G[3]

.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
        
