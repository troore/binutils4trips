; Test:     store_blitz_0
; Author:   Karu
; Purpose:  32 Stores all to different addresses
;           Does not test tsim_arch

; Final result should be 0

.set SYS_EXIT=1

.data
testdata:
.fill 1024, 1, 0

.text
.global _start
.bbegin _start

N[1] gens 0 W[3]
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
N[80] sd 128
N[81] sd 136
N[82] sd 144
N[83] sd 152
N[84] sd 160
N[85] sd 168
N[86] sd 176
N[87] sd 184
N[88] sd 192
N[89] sd 200
N[90] sd 208
N[91] sd 216
N[92] sd 224
N[93] sd 232
N[94] sd 240
N[95] sd 248

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
N[112] gens 16 N[80,1]
N[113] gens 17 N[81,1]
N[114] gens 18 N[82,1]
N[115] gens 19 N[83,1]
N[116] gens 20 N[84,1]
N[117] gens 21 N[85,1]
N[118] gens 22 N[86,1]
N[119] gens 23 N[87,1]
N[120] gens 24 N[88,1]
N[121] gens 25 N[89,1]
N[122] gens 26 N[90,1]
N[123] gens 27 N[91,1]
N[124] gens 28 N[92,1]
N[125] gens 29 N[93,1]
N[126] gens 30 N[94,1]
N[127] gens 31 N[95,1]


N[0] bro exit
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
