; .app-file "test.c"
.data	
.align	8
_V0:
.ascii	"a[%d] = %5d\n\000"
.text	
.global	main
; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main
;;;;;;;;;;; Begin read preamble
R[1] read G[1] N[5,1] N[1,0]
R[2] read G[2] N[9,1]
R[0] read G[12] N[0,1]
;;;;;;;;;;; End read preamble
N[1] <0> mov N[2,0] N[0,0]
N[2] <1> addi -255 N[6,0]
N[6] <2> mov N[7,0] N[5,0]
N[7] <3> addi -241 N[3,0]
N[3] <4> mov N[11,0] W[1]
N[11] <5> mov N[10,0] N[9,0]
N[5] <6> sd S[0] -241
N[9] <7> sd S[1] 8
N[0] <8> sd S[2] -8
N[10] <9> addi 88 W[2]
N[15] <10> movi 0 W[3]
N[4] <11> bro B[0] main$1
;;;;;;;;;;; Begin write epilogue
W[1] write G[1]
W[2] write G[70]
W[3] write G[71]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$1
;;;;;;;;;;; Begin read preamble
R[2] read G[70] N[14,0] N[46,0]
R[3] read G[71] N[6,0] N[7,0]
;;;;;;;;;;; End read preamble
N[7] <0> mov N[3,0] N[23,0]
N[6] <1> mov3 N[4,0] N[5,0] N[2,0]
N[23] <2> mov N[39,0] N[10,1]
N[3] <3> mov3 N[19,0] N[35,0] N[51,0]
N[2] <4> mov3 N[1,0] N[18,0] N[17,0]
N[5] <5> mov3 N[20,0] N[21,0] N[9,0]
N[4] <6> mov3 N[8,0] N[24,0] N[10,0]
N[14] <7> mov3 N[15,0] N[30,0] N[29,0]
N[46] <8> mov3 N[59,0] N[58,0] N[63,0]
N[29] <9> mov3 N[52,0] N[49,0] N[48,0]
N[30] <10> mov3 N[53,0] N[54,0] N[57,0]
N[15] <11> mov3 N[44,0] N[60,0] N[42,0]
N[10] <12> mul N[42,1]
N[42] <13> sw S[0]
N[8] <14> addi 1 N[12,0]
N[12] <15> extsw N[28,0] N[28,1]
N[28] <16> mul N[44,1]
N[44] <17> sw S[1] 4
N[24] <18> addi 2 N[40,0]
N[40] <19> extsw N[56,0] N[56,1]
N[56] <20> mul N[60,1]
N[60] <21> sw S[2] 8
N[20] <22> addi 3 N[16,0]
N[16] <23> extsw N[37,0] N[37,1]
N[37] <24> mul N[53,1]
N[53] <25> sw S[3] 12
N[21] <26> addi 4 N[22,0]
N[22] <27> extsw N[38,0] N[38,1]
N[38] <28> mul N[54,1]
N[54] <29> sw S[4] 16
N[9] <30> addi 5 N[25,0]
N[25] <31> extsw N[41,0] N[41,1]
N[41] <32> mul N[57,1]
N[57] <33> sw S[5] 20
N[1] <34> addi 6 N[0,0]
N[0] <35> extsw N[36,0] N[36,1]
N[36] <36> mul N[52,1]
N[52] <37> sw S[6] 24
N[18] <38> addi 7 N[34,0]
N[34] <39> extsw N[50,0] N[50,1]
N[50] <40> mul N[49,1]
N[49] <41> sw S[7] 28
N[17] <42> addi 8 N[33,0]
N[33] <43> extsw N[32,0] N[32,1]
N[32] <44> mul N[48,1]
N[48] <45> sw S[8] 32
N[19] <46> addi 9 N[11,0]
N[11] <47> extsw N[43,0] N[43,1]
N[43] <48> mul N[59,1]
N[59] <49> sw S[9] 36
N[35] <50> addi 10 N[27,0]
N[27] <51> extsw N[26,0] N[26,1]
N[26] <52> mul N[58,1]
N[58] <53> sw S[10] 40
N[51] <54> addi 11 N[31,0]
N[31] <55> extsw N[47,0] N[47,1]
N[47] <56> mul N[63,1]
N[63] <57> sw S[11] 44
N[39] <58> addi 12 N[55,0]
N[55] <59> extsw W[0]
N[13] <60> bro B[0] main$2
;;;;;;;;;;; Begin write epilogue
W[0] write G[72]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$2
;;;;;;;;;;; Begin read preamble
R[2] read G[70] N[12,0] N[14,0]
R[3] read G[71] N[7,0] N[3,0]
R[0] read G[72] N[8,0]
;;;;;;;;;;; End read preamble
N[7] <0> mov3 N[6,0] N[23,0] N[11,0]
N[3] <1> mov3 N[2,0] N[19,0] N[0,0]
N[12] <2> mov3 N[24,0] N[29,0] N[26,0]
N[14] <3> mov3 N[17,0] N[20,0] N[21,0]
N[2] <4> addi 12 N[22,0]
N[22] <5> extsw N[8,1]
N[8] <6> mul N[24,1]
N[24] <7> sw S[0] 48
N[19] <8> addi 13 N[18,0]
N[18] <9> extsw N[1,0] N[1,1]
N[1] <10> mul N[17,1]
N[17] <11> sw S[1] 52
N[0] <12> addi 14 N[16,0]
N[16] <13> extsw N[4,0] N[4,1]
N[4] <14> mul N[20,1]
N[20] <15> sw S[2] 56
N[6] <16> addi 15 N[9,0]
N[9] <17> extsw N[25,0] N[25,1]
N[25] <18> mul N[21,1]
N[21] <19> sw S[3] 60
N[23] <20> addi 16 N[5,0]
N[5] <21> extsw N[13,0] N[13,1]
N[13] <22> mul N[29,1]
N[29] <23> sw S[4] 64
N[11] <24> addi 17 N[27,0]
N[27] <25> extsw N[10,0] N[10,1]
N[10] <26> mul N[26,1]
N[26] <27> sw S[5] 68
N[15] <28> bro B[0] main$3
;;;;;;;;;;; Begin write epilogue
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$3
;;;;;;;;;;; Begin read preamble
R[2] read G[70] N[6,0] N[14,0]
R[3] read G[71] N[3,0] N[7,0]
;;;;;;;;;;; End read preamble
N[3] <0> mov N[2,0] N[19,0]
N[7] <1> mov3 N[23,0] N[15,0] N[9,0]
N[19] <2> mov3 N[35,0] N[11,0] N[27,0]
N[2] <3> mov3 N[1,0] N[18,0] N[0,0]
N[6] <4> mov3 N[22,0] N[13,0] N[40,0]
N[14] <5> mov3 N[36,0] N[41,0] N[42,0]
N[13] <6> mov3 N[37,0] N[33,0] N[20,0]
N[1] <7> addi 18 N[5,0]
N[5] <8> extsw N[21,0] N[21,1]
N[21] <9> mul N[37,1]
N[37] <10> sw S[0] 72
N[18] <11> addi 19 N[34,0]
N[34] <12> extsw N[17,0] N[17,1]
N[17] <13> mul N[33,1]
N[33] <14> sw S[1] 76
N[0] <15> addi 20 N[16,0]
N[16] <16> extsw N[32,0] N[32,1]
N[32] <17> mul N[20,1]
N[20] <18> sw S[2] 80
N[35] <19> addi 21 N[38,0]
N[38] <20> extsw N[4,0] N[4,1]
N[4] <21> mul N[36,1]
N[36] <22> sw S[3] 84
N[11] <23> addi 22 N[10,0]
N[10] <24> extsw N[25,0] N[25,1]
N[25] <25> mul N[41,1]
N[41] <26> sw S[4] 88
N[27] <27> addi 23 N[43,0]
N[43] <28> extsw N[26,0] N[26,1]
N[26] <29> mul N[42,1]
N[42] <30> sw S[5] 92
N[9] <31> addi 24 N[8,0]
N[8] <32> extsw N[24,0] N[24,1]
N[24] <33> mul N[40,1]
N[40] <34> sw S[6] 96
N[23] <35> addi 25 N[39,0]
N[39] <36> extsw W[3]
N[22] <37> addi 100 W[2]
N[15] <38> addi 25 N[31,0]
N[31] <39> extsw N[47,0]
N[47] <40> tlti 76 N[30,0]
N[30] <41> mov N[46,p] N[29,p]
N[46] <42> bro_t B[0] main$1
N[29] <43> bro_f B[1] main$4
;;;;;;;;;;; Begin write epilogue
W[2] write G[70]
W[3] write G[71]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$4
;;;;;;;;;;; Begin read preamble
R[1] read G[1] N[1,0]
;;;;;;;;;;; End read preamble
N[1] <0> addi 88 N[0,0]
N[4] <1> movi 123 N[0,1]
N[0] <2> sw S[0] 92
N[12] <3> movi 0 W[0]
N[5] <4> bro B[0] main$5
;;;;;;;;;;; Begin write epilogue
W[0] write G[12]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$5
;;;;;;;;;;; Begin read preamble
R[1] read G[1] N[1,0]
R[0] read G[12] N[12,0] N[8,0]
;;;;;;;;;;; End read preamble
N[8] <0> mov W[0]
N[1] <1> addi 88 N[4,1]
N[12] <2> slli 2 N[4,0]
N[4] <3> add N[0,0]
N[0] <4> lws L[0] W[1]
N[3] <5> genu %lo(_V0) N[2,0]
N[2] <6> app %bottom(_V0) W[3]
N[6] <7> genu %lo(main$6) N[7,0]
N[7] <8> app %bottom(main$6) W[2]
N[5] <9> callo B[0] printf
;;;;;;;;;;; Begin write epilogue
W[2] write G[2]
W[3] write G[3]
W[0] write G[4]
W[1] write G[5]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$6
;;;;;;;;;;; Begin read preamble
R[0] read G[12] N[12,0]
;;;;;;;;;;; End read preamble
N[12] <0> addi 1 N[8,0]
N[8] <1> extsw N[4,0] W[0]
N[4] <2> tlti 100 N[5,0]
N[5] <3> mov N[1,p] N[6,p]
N[1] <4> bro_t B[0] main$5
N[6] <5> bro_f B[1] main$7
;;;;;;;;;;; Begin write epilogue
W[0] write G[12]
;;;;;;;;;;; End write epilogue
.bend

; Scheduler info: Fanout algorithm: Huffman with max mov3s
; Scheduler info: Scheduling algorithm: RBA

.bbegin main$7
;;;;;;;;;;; Begin read preamble
R[1] read G[1] N[0,0] N[1,0]
;;;;;;;;;;; End read preamble
N[7] <0> movi 0 W[3]
N[0] <1> ld L[0] 8 N[4,0]
N[4] <2> mov N[5,0] W[2]
N[1] <3> addi 255 N[2,0]
N[2] <4> mov N[3,0] N[6,0]
N[6] <5> addi 241 W[1]
N[3] <6> ld L[1] 233 W[0]
N[5] <7> ret B[0]
;;;;;;;;;;; Begin write epilogue
W[1] write G[1]
W[2] write G[2]
W[3] write G[3]
W[0] write G[12]
;;;;;;;;;;; End write epilogue
.bend

