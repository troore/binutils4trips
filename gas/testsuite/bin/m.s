; .app-file "m.c"
.global a
.comm	a, 4096, 4
.global b
.comm	b, 4096, 4
.global c
.comm	c, 4096, 4
.text	
.global	matrix_multiply
; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin matrix_multiply
;;;;;;;;;;; Begin read preamble
R[1] read G[1] N[1,0] N[4,1]
;;;;;;;;;;; End read preamble
N[1] <0> addi -32 N[0,0]
N[0] <1> mov N[4,0] W[1]
N[4] <2> sd S[0]
N[11] <3> movi 0 W[3]
N[5] <4> bro B[0] matrix_multiply$1
;;;;;;;;;;; Begin write epilogue
W[1] write G[1]
W[3] write G[75]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin matrix_multiply$1
;;;;;;;;;;; Begin read preamble
R[3] read G[75] N[11,0]
;;;;;;;;;;; End read preamble
N[6] <0> movi 0 W[2]
N[11] <1> slli 5 W[1]
N[5] <2> bro B[0] matrix_multiply$2
;;;;;;;;;;; Begin write epilogue
W[2] write G[70]
W[1] write G[77]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin matrix_multiply$2
;;;;;;;;;;; Begin read preamble
R[2] read G[70] N[20,1] N[6,0]
R[1] read G[77] N[20,0] N[13,0]
;;;;;;;;;;; End read preamble
N[20] <0> add N[36,0]
N[8] <1> genu %lo(c) N[24,0]
N[24] <2> app %bottom(c) N[40,1]
N[36] <3> slli 2 N[40,0]
N[40] <4> add W[1]
N[14] <5> movi 0 N[26,0]
N[7] <6> genu %lo(b) N[35,0]
N[35] <7> app %bottom(b) N[22,1]
N[6] <8> slli 2 N[22,0]
N[22] <9> add N[38,0] N[2,0]
N[38] <10> mov N[54,0] N[21,0]
N[2] <11> mov N[51,0] W[3]
N[54] <12> mov N[70,0] N[18,0]
N[21] <13> mov3 N[37,0] N[1,0] N[52,0]
N[51] <14> mov3 N[67,0] N[33,0] N[63,0]
N[3] <15> genu %lo(a) N[19,0]
N[19] <16> app %bottom(a) N[11,1]
N[13] <17> slli 2 N[11,0]
N[11] <18> add N[27,0] N[9,0]
N[27] <19> mov N[43,0] N[15,0]
N[9] <20> mov N[25,0] W[0]
N[43] <21> mov N[23,0] N[39,0]
N[15] <22> mov3 N[31,0] N[30,0] N[47,0]
N[25] <23> mov3 N[56,0] N[41,0] N[57,0]
N[23] <24> lw L[0] N[55,0]
N[55] <25> fstod N[10,0]
N[70] <26> lw L[1] N[86,0]
N[86] <27> fstod N[10,1]
N[10] <28> fmul N[26,1]
N[26] <29> fadd N[42,0]
N[39] <30> lw L[2] 4 N[71,0]
N[71] <31> fstod N[87,0]
N[18] <32> lw L[3] 128 N[34,0]
N[34] <33> fstod N[87,1]
N[87] <34> fmul N[42,1]
N[42] <35> fadd N[58,0]
N[31] <36> lw L[4] 8 N[79,0]
N[79] <37> fstod N[89,0]
N[37] <38> addi 1 N[53,0]
N[53] <39> lw L[5] 255 N[69,0]
N[69] <40> fstod N[89,1]
N[89] <41> fmul N[58,1]
N[58] <42> fadd N[74,0]
N[30] <43> lw L[6] 12 N[46,0]
N[46] <44> fstod N[66,0]
N[1] <45> addi 129 N[17,0]
N[17] <46> lw L[7] 255 N[65,0]
N[65] <47> fstod N[66,1]
N[66] <48> fmul N[74,1]
N[74] <49> fadd N[90,0]
N[47] <50> lw L[8] 16 N[62,0]
N[62] <51> fstod N[88,0]
N[4] <52> genu 257 N[52,1]
N[52] <53> add N[68,0]
N[68] <54> lw L[9] 255 N[84,0]
N[84] <55> fstod N[88,1]
N[88] <56> fmul N[90,1]
N[90] <57> fadd N[59,0]
N[56] <58> lw L[10] 20 N[72,0]
N[72] <59> fstod N[82,0]
N[0] <60> genu 385 N[67,1]
N[67] <61> add N[83,0]
N[83] <62> lw L[11] 255 N[50,0]
N[50] <63> fstod N[82,1]
N[82] <64> fmul N[59,1]
N[59] <65> fadd N[75,0]
N[41] <66> lw L[12] 24 N[73,0]
N[73] <67> fstod N[85,0]
N[5] <68> genu 513 N[33,1]
N[33] <69> add N[49,0]
N[49] <70> lw L[13] 255 N[81,0]
N[81] <71> fstod N[85,1]
N[85] <72> fmul N[75,1]
N[75] <73> fadd N[91,0]
N[57] <74> lw L[14] 28 N[29,0]
N[29] <75> fstod N[94,0]
N[12] <76> genu 641 N[63,1]
N[63] <77> add N[95,0]
N[95] <78> lw L[15] 255 N[78,0]
N[78] <79> fstod N[94,1]
N[94] <80> fmul N[91,1]
N[91] <81> fadd W[2]
N[16] <82> bro B[0] matrix_multiply$3
;;;;;;;;;;; Begin write epilogue
W[3] write G[71]
W[0] write G[72]
W[1] write G[73]
W[2] write G[74]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin matrix_multiply$3
;;;;;;;;;;; Begin read preamble
R[3] read G[71] N[7,0] N[11,0]
R[0] read G[72] N[24,0] N[9,0]
R[2] read G[74] N[10,0]
;;;;;;;;;;; End read preamble
N[7] <0> mov N[23,0] N[6,0]
N[11] <1> mov3 N[27,0] N[15,0] N[3,0]
N[6] <2> mov3 N[22,0] N[37,0] N[18,0]
N[24] <3> mov N[40,0] N[20,0]
N[9] <4> mov3 N[25,0] N[29,0] N[21,0]
N[20] <5> mov3 N[0,0] N[36,0] N[16,0]
N[40] <6> lw L[0] 32 N[56,0]
N[56] <7> fstod N[54,0]
N[2] <8> genu 769 N[23,1]
N[23] <9> add N[39,0]
N[39] <10> lw L[1] 255 N[55,0]
N[55] <11> fstod N[54,1]
N[54] <12> fmul N[10,1]
N[10] <13> fadd N[26,0]
N[25] <14> lw L[2] 36 N[41,0]
N[41] <15> fstod N[63,0]
N[13] <16> genu 897 N[27,1]
N[27] <17> add N[43,0]
N[43] <18> lw L[3] 255 N[59,0]
N[59] <19> fstod N[63,1]
N[63] <20> fmul N[26,1]
N[26] <21> fadd N[42,0]
N[29] <22> lw L[4] 40 N[45,0]
N[45] <23> fstod N[14,0]
N[12] <24> genu 1025 N[15,1]
N[15] <25> add N[31,0]
N[31] <26> lw L[5] 255 N[47,0]
N[47] <27> fstod N[14,1]
N[14] <28> fmul N[42,1]
N[42] <29> fadd N[58,0]
N[21] <30> lw L[6] 44 N[53,0]
N[53] <31> fstod N[51,0]
N[1] <32> genu 1153 N[3,1]
N[3] <33> add N[19,0]
N[19] <34> lw L[7] 255 N[35,0]
N[35] <35> fstod N[51,1]
N[51] <36> fmul N[58,1]
N[58] <37> fadd N[30,0]
N[0] <38> lw L[8] 48 N[32,0]
N[32] <39> fstod N[50,0]
N[5] <40> genu 1281 N[22,1]
N[22] <41> add N[38,0]
N[38] <42> lw L[9] 255 N[34,0]
N[34] <43> fstod N[50,1]
N[50] <44> fmul N[30,1]
N[30] <45> fadd N[46,0]
N[36] <46> lw L[10] 52 N[52,0]
N[52] <47> fstod N[44,0]
N[4] <48> genu 1409 N[37,1]
N[37] <49> add N[57,0]
N[57] <50> lw L[11] 255 N[61,0]
N[61] <51> fstod N[44,1]
N[44] <52> fmul N[46,1]
N[46] <53> fadd N[62,0]
N[16] <54> lw L[12] 56 N[48,0]
N[48] <55> fstod N[49,0]
N[8] <56> genu 1537 N[18,1]
N[18] <57> add N[17,0]
N[17] <58> lw L[13] 255 N[33,0]
N[33] <59> fstod N[49,1]
N[49] <60> fmul N[62,1]
N[62] <61> fadd W[0]
N[28] <62> bro B[0] matrix_multiply$4
;;;;;;;;;;; Begin write epilogue
W[0] write G[76]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin matrix_multiply$4
;;;;;;;;;;; Begin read preamble
R[3] read G[71] N[7,0] N[27,0]
R[0] read G[72] N[8,0] N[10,0]
R[4] read G[76] N[12,0]
;;;;;;;;;;; End read preamble
N[8] <0> mov N[9,0] N[4,0]
N[10] <1> mov3 N[43,0] N[26,0] N[6,0]
N[4] <2> mov3 N[20,0] N[0,0] N[16,0]
N[7] <3> mov N[23,0] N[5,0]
N[27] <4> mov3 N[31,0] N[3,0] N[42,0]
N[5] <5> mov3 N[21,0] N[1,0] N[36,0]
N[9] <6> lw L[0] 60 N[25,0]
N[25] <7> fstod N[53,0]
N[11] <8> genu 1665 N[23,1]
N[23] <9> add N[39,0]
N[39] <10> lw L[1] 255 N[55,0]
N[55] <11> fstod N[53,1]
N[53] <12> fmul N[12,1]
N[12] <13> fadd N[28,0]
N[43] <14> lw L[2] 64 N[59,0]
N[59] <15> fstod N[63,0]
N[15] <16> genu 1793 N[31,1]
N[31] <17> add N[47,0]
N[47] <18> lw L[3] 255 N[46,0]
N[46] <19> fstod N[63,1]
N[63] <20> fmul N[28,1]
N[28] <21> fadd N[44,0]
N[26] <22> lw L[4] 68 N[58,0]
N[58] <23> fstod N[51,0]
N[2] <24> genu 1921 N[3,1]
N[3] <25> add N[19,0]
N[19] <26> lw L[5] 255 N[35,0]
N[35] <27> fstod N[51,1]
N[51] <28> fmul N[44,1]
N[44] <29> fadd N[60,0]
N[6] <30> lw L[6] 72 N[18,0]
N[18] <31> fstod N[54,0]
N[14] <32> genu 2049 N[42,1]
N[42] <33> add N[22,0]
N[22] <34> lw L[7] 255 N[38,0]
N[38] <35> fstod N[54,1]
N[54] <36> fmul N[60,1]
N[60] <37> fadd N[24,0]
N[20] <38> lw L[8] 76 N[52,0]
N[52] <39> fstod N[57,0]
N[13] <40> genu 2177 N[21,1]
N[21] <41> add N[37,0]
N[37] <42> lw L[9] 255 N[41,0]
N[41] <43> fstod N[57,1]
N[57] <44> fmul N[24,1]
N[24] <45> fadd N[40,0]
N[0] <46> lw L[10] 80 N[48,0]
N[48] <47> fstod N[49,0]
N[29] <48> genu 2305 N[1,1]
N[1] <49> add N[17,0]
N[17] <50> lw L[11] 255 N[33,0]
N[33] <51> fstod N[49,1]
N[49] <52> fmul N[40,1]
N[40] <53> fadd N[56,0]
N[16] <54> lw L[12] 84 N[61,0]
N[61] <55> fstod N[50,0]
N[30] <56> genu 2433 N[36,1]
N[36] <57> add N[32,0]
N[32] <58> lw L[13] 255 N[34,0]
N[34] <59> fstod N[50,1]
N[50] <60> fmul N[56,1]
N[56] <61> fadd W[2]
N[45] <62> bro B[0] matrix_multiply$5
;;;;;;;;;;; Begin write epilogue
W[2] write G[74]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin matrix_multiply$5
;;;;;;;;;;; Begin read preamble
R[2] read G[70] N[2,0]
R[3] read G[71] N[7,0] N[19,0]
R[0] read G[72] N[8,0] N[20,0]
R[1] read G[73] N[86,0]
R[6] read G[74] N[10,0]
;;;;;;;;;;; End read preamble
N[7] <0> mov3 N[23,0] N[27,0] N[31,0]
N[19] <1> mov3 N[35,0] N[34,0] N[67,0]
N[27] <2> mov3 N[43,0] N[41,0] N[37,0]
N[31] <3> mov3 N[14,0] N[47,0] N[45,0]
N[8] <4> mov3 N[9,0] N[24,0] N[60,0]
N[20] <5> mov3 N[5,0] N[36,0] N[21,0]
N[24] <6> mov3 N[25,0] N[40,0] N[52,0]
N[60] <7> mov3 N[29,0] N[76,0] N[56,0]
N[9] <8> lw L[0] 88 N[73,0]
N[73] <9> fstod N[75,0]
N[3] <10> genu 2561 N[23,1]
N[23] <11> add N[39,0]
N[39] <12> lw L[1] 255 N[71,0]
N[71] <13> fstod N[75,1]
N[75] <14> fmul N[10,1]
N[10] <15> fadd N[26,0]
N[5] <16> lw L[2] 92 N[53,0]
N[53] <17> fstod N[82,0]
N[11] <18> genu 2689 N[35,1]
N[35] <19> add N[51,0]
N[51] <20> lw L[3] 255 N[83,0]
N[83] <21> fstod N[82,1]
N[82] <22> fmul N[26,1]
N[26] <23> fadd N[42,0]
N[36] <24> lw L[4] 96 N[68,0]
N[68] <25> fstod N[49,0]
N[1] <26> genu 2817 N[34,1]
N[34] <27> add N[50,0]
N[50] <28> lw L[5] 255 N[66,0]
N[66] <29> fstod N[49,1]
N[49] <30> fmul N[42,1]
N[42] <31> fadd N[58,0]
N[21] <32> lw L[6] 100 N[69,0]
N[69] <33> fstod N[91,0]
N[0] <34> genu 2945 N[67,1]
N[67] <35> add N[55,0]
N[55] <36> lw L[7] 255 N[87,0]
N[87] <37> fstod N[91,1]
N[91] <38> fmul N[58,1]
N[58] <39> fadd N[74,0]
N[25] <40> lw L[8] 104 N[89,0]
N[89] <41> fstod N[62,0]
N[15] <42> genu 3073 N[43,1]
N[43] <43> add N[59,0]
N[59] <44> lw L[9] 255 N[63,0]
N[63] <45> fstod N[62,1]
N[62] <46> fmul N[74,1]
N[74] <47> fadd N[90,0]
N[40] <48> lw L[10] 108 N[72,0]
N[72] <49> fstod N[93,0]
N[13] <50> genu 3201 N[41,1]
N[41] <51> add N[57,0]
N[57] <52> lw L[11] 255 N[77,0]
N[77] <53> fstod N[93,1]
N[93] <54> fmul N[90,1]
N[90] <55> fadd N[6,0]
N[52] <56> lw L[12] 112 N[84,0]
N[84] <57> fstod N[85,0]
N[4] <58> genu 3329 N[37,1]
N[37] <59> add N[17,0]
N[17] <60> lw L[13] 255 N[65,0]
N[65] <61> fstod N[85,1]
N[85] <62> fmul N[6,1]
N[6] <63> fadd N[22,0]
N[29] <64> lw L[14] 116 N[61,0]
N[61] <65> fstod N[95,0]
N[12] <66> genu 3457 N[14,1]
N[14] <67> add N[30,0]
N[30] <68> lw L[15] 255 N[79,0]
N[79] <69> fstod N[95,1]
N[95] <70> fmul N[22,1]
N[22] <71> fadd N[38,0]
N[76] <72> lw L[16] 120 N[92,0]
N[92] <73> fstod N[94,0]
N[28] <74> genu 3585 N[47,1]
N[47] <75> add N[46,0]
N[46] <76> lw L[17] 255 N[78,0]
N[78] <77> fstod N[94,1]
N[94] <78> fmul N[38,1]
N[38] <79> fadd N[54,0]
N[56] <80> lw L[18] 124 N[88,0]
N[88] <81> fstod N[80,0]
N[44] <82> genu 3713 N[45,1]
N[45] <83> add N[33,0]
N[33] <84> lw L[19] 255 N[81,0]
N[81] <85> fstod N[80,1]
N[80] <86> fmul N[54,1]
N[54] <87> fadd N[70,0]
N[70] <88> fdtos N[86,1]
N[86] <89> sw S[20]
N[2] <90> addi 1 N[18,0]
N[18] <91> extsw N[16,0] W[2]
N[16] <92> tlti 32 N[32,0]
N[32] <93> mov N[48,p] N[64,p]
N[48] <94> bro_t B[0] matrix_multiply$2
N[64] <95> bro_f B[1] matrix_multiply$6
;;;;;;;;;;; Begin write epilogue
W[2] write G[70]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin matrix_multiply$6
;;;;;;;;;;; Begin read preamble
R[3] read G[75] N[10,0]
;;;;;;;;;;; End read preamble
N[10] <0> addi 1 N[6,0]
N[6] <1> extsw N[5,0] W[3]
N[5] <2> tlti 32 N[1,0]
N[1] <3> mov N[2,p] N[0,p]
N[2] <4> bro_t B[0] matrix_multiply$1
N[0] <5> bro_f B[1] matrix_multiply$7
;;;;;;;;;;; Begin write epilogue
W[3] write G[75]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin matrix_multiply$7
;;;;;;;;;;; Begin read preamble
R[1] read G[1] N[1,0]
R[2] read G[2] N[2,0]
;;;;;;;;;;; End read preamble
N[1] <0> addi 32 W[1]
N[2] <1> ret B[0]
;;;;;;;;;;; Begin write epilogue
W[1] write G[1]
;;;;;;;;;;; End write epilogue
.bend

.global	main
; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main
;;;;;;;;;;; Begin read preamble
R[1] read G[1] N[7,0] N[1,0]
R[2] read G[2] N[9,1]
R[0] read G[12] N[10,1]
R[5] read G[13] N[8,1]
R[6] read G[14] N[2,1]
R[3] read G[15] N[3,1]
;;;;;;;;;;; End read preamble
N[7] <0> mov N[11,0] N[6,1]
N[1] <1> mov N[0,0] N[3,0]
N[11] <2> mov3 N[2,0] N[8,0] N[10,0]
N[0] <3> addi -128 N[4,0]
N[4] <4> mov N[5,0] W[1]
N[5] <5> mov N[6,0] N[9,0]
N[6] <6> sd S[0]
N[9] <7> sd S[1] 8
N[10] <8> sd S[2] -32
N[8] <9> sd S[3] -24
N[2] <10> sd S[4] -16
N[3] <11> sd S[5] -8
N[15] <12> movi 0 W[3]
N[14] <13> genu %lo(main$1) N[13,0]
N[13] <14> app %bottom(main$1) W[2]
N[12] <15> callo B[0] time
;;;;;;;;;;; Begin write epilogue
W[1] write G[1]
W[2] write G[2]
W[3] write G[3]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$1
;;;;;;;;;;; Begin read preamble
R[3] read G[3] N[7,0]
;;;;;;;;;;; End read preamble
N[7] <0> extuw W[3]
N[2] <1> genu %lo(main$2) N[3,0]
N[3] <2> app %bottom(main$2) W[2]
N[5] <3> callo B[0] srand
;;;;;;;;;;; Begin write epilogue
W[2] write G[2]
W[3] write G[3]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$2
;;;;;;;;;;; Begin read preamble
;;;;;;;;;;; End read preamble
N[13] <0> movi 0 W[1]
N[5] <1> bro B[0] main$3
;;;;;;;;;;; Begin write epilogue
W[1] write G[13]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$3
;;;;;;;;;;; Begin read preamble
;;;;;;;;;;; End read preamble
N[12] <0> movi 0 W[0]
N[5] <1> bro B[0] main$4
;;;;;;;;;;; Begin write epilogue
W[0] write G[12]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$4
;;;;;;;;;;; Begin read preamble
;;;;;;;;;;; End read preamble
N[2] <0> genu %lo(main$5) N[3,0]
N[3] <1> app %bottom(main$5) W[2]
N[5] <2> callo B[0] rand
;;;;;;;;;;; Begin write epilogue
W[2] write G[2]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$5
;;;;;;;;;;; Begin read preamble
R[3] read G[3] N[3,0]
;;;;;;;;;;; End read preamble
N[3] <0> mov W[3]
N[2] <1> genu %lo(main$6) N[1,0]
N[1] <2> app %bottom(main$6) W[2]
N[5] <3> callo B[0] rand
;;;;;;;;;;; Begin write epilogue
W[2] write G[2]
W[3] write G[15]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$6
;;;;;;;;;;; Begin read preamble
R[3] read G[3] N[3,0]
R[0] read G[12] N[14,1]
R[1] read G[13] N[13,0]
R[7] read G[15] N[15,0]
;;;;;;;;;;; End read preamble
N[13] <0> slli 5 N[14,0]
N[14] <1> add N[10,0]
N[6] <2> genu %lo(a) N[7,0]
N[7] <3> app %bottom(a) N[11,1]
N[10] <4> slli 2 N[11,0]
N[11] <5> add W[6]
N[15] <6> fitod W[3]
N[3] <7> fitod W[0]
N[2] <8> genu %lo(main$7) N[1,0]
N[1] <9> app %bottom(main$7) W[2]
N[5] <10> callo B[0] _float64_div
;;;;;;;;;;; Begin write epilogue
W[2] write G[2]
W[3] write G[3]
W[0] write G[4]
W[6] write G[14]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$7
;;;;;;;;;;; Begin read preamble
R[3] read G[3] N[3,0]
R[2] read G[14] N[2,0]
;;;;;;;;;;; End read preamble
N[3] <0> fdtos N[2,1]
N[2] <1> sw S[0]
N[1] <2> genu %lo(main$8) N[0,0]
N[0] <3> app %bottom(main$8) W[2]
N[5] <4> callo B[0] rand
;;;;;;;;;;; Begin write epilogue
W[2] write G[2]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$8
;;;;;;;;;;; Begin read preamble
R[3] read G[3] N[3,0]
;;;;;;;;;;; End read preamble
N[3] <0> mov W[3]
N[2] <1> genu %lo(main$9) N[1,0]
N[1] <2> app %bottom(main$9) W[2]
N[5] <3> callo B[0] rand
;;;;;;;;;;; Begin write epilogue
W[2] write G[2]
W[3] write G[15]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$9
;;;;;;;;;;; Begin read preamble
R[3] read G[3] N[3,0]
R[0] read G[12] N[12,0]
R[1] read G[13] N[13,1]
R[7] read G[15] N[15,0]
;;;;;;;;;;; End read preamble
N[12] <0> slli 5 N[13,0]
N[13] <1> add N[14,0]
N[10] <2> genu %lo(b) N[11,0]
N[11] <3> app %bottom(b) N[6,1]
N[14] <4> slli 2 N[6,0]
N[6] <5> add W[6]
N[15] <6> fitod W[3]
N[3] <7> fitod W[0]
N[2] <8> genu %lo(main$10) N[1,0]
N[1] <9> app %bottom(main$10) W[2]
N[5] <10> callo B[0] _float64_div
;;;;;;;;;;; Begin write epilogue
W[2] write G[2]
W[3] write G[3]
W[0] write G[4]
W[6] write G[14]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$10
;;;;;;;;;;; Begin read preamble
R[3] read G[3] N[3,0]
R[0] read G[12] N[12,0]
R[2] read G[14] N[2,0]
;;;;;;;;;;; End read preamble
N[3] <0> fdtos N[2,1]
N[2] <1> sw S[0]
N[12] <2> addi 1 N[8,0]
N[8] <3> extsw N[4,0] W[0]
N[4] <4> tlti 32 N[5,0]
N[5] <5> mov N[1,p] N[6,p]
N[1] <6> bro_t B[0] main$4
N[6] <7> bro_f B[1] main$11
;;;;;;;;;;; Begin write epilogue
W[0] write G[12]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$11
;;;;;;;;;;; Begin read preamble
R[1] read G[13] N[13,0]
;;;;;;;;;;; End read preamble
N[13] <0> addi 1 N[9,0]
N[9] <1> extsw N[5,0] W[1]
N[5] <2> tlti 32 N[1,0]
N[1] <3> mov N[2,p] N[0,p]
N[2] <4> bro_t B[0] main$3
N[0] <5> bro_f B[1] main$12
;;;;;;;;;;; Begin write epilogue
W[1] write G[13]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$12
;;;;;;;;;;; Begin read preamble
;;;;;;;;;;; End read preamble
N[2] <0> genu %lo(main$13) N[3,0]
N[3] <1> app %bottom(main$13) W[2]
N[5] <2> callo B[0] matrix_multiply
;;;;;;;;;;; Begin write epilogue
W[2] write G[2]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$13
;;;;;;;;;;; Begin read preamble
R[1] read G[1] N[0,0] N[1,0]
;;;;;;;;;;; End read preamble
N[15] <0> movi 0 W[3]
N[0] <1> ld L[0] 8 N[4,0]
N[4] <2> mov N[8,0] W[2]
N[1] <3> addi 128 N[2,0]
N[2] <4> mov N[3,0] N[6,0]
N[3] <5> mov N[11,0] W[1]
N[6] <6> mov3 N[7,0] N[10,0] N[5,0]
N[11] <7> ld L[1] -32 W[0]
N[7] <8> ld L[2] -24 W[5]
N[10] <9> ld L[3] -16 W[6]
N[5] <10> ld L[4] -8 W[7]
N[8] <11> ret B[0]
;;;;;;;;;;; Begin write epilogue
W[1] write G[1]
W[2] write G[2]
W[3] write G[3]
W[0] write G[12]
W[5] write G[13]
W[6] write G[14]
W[7] write G[15]
;;;;;;;;;;; End write epilogue
.bend

