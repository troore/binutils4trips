; Test :       opn_test_1
; Author :     Karu
; Purpose :  Simple operand network forwarding test with N[0] chained to N[1]
;            N[1] chained to N[2] and so on...through a single cycle add
; Final Result must be 0x220

.set SYS_EXIT=1

.text
.global _start

.bbegin _start
N[0] gens 40 N[1, 0]         
N[1] addi 4 N[2, 0]
N[2] addi 4 N[3, 0]
N[3] addi 4 N[4, 0]
N[4] addi 4 N[5, 0]
N[5] addi 4 N[6, 0]
N[6] addi 4 N[7, 0]
N[7] addi 4 N[8, 0]
N[8] addi 4 N[9, 0]
N[9] addi 4 N[10, 0]
N[10] addi 4 N[11, 0]
N[11] addi 4 N[12, 0]
N[12] addi 4 N[13, 0]
N[13] addi 4 N[14, 0]
N[14] addi 4 N[15, 0]
N[15] addi 4 N[16, 0]
N[16] addi 4 N[17, 0]
N[17] addi 4 N[18, 0]
N[18] addi 4 N[19, 0]
N[19] addi 4 N[20, 0]
N[20] addi 4 N[21, 0]
N[21] addi 4 N[22, 0]
N[22] addi 4 N[23, 0]
N[23] addi 4 N[24, 0]
N[24] addi 4 N[25, 0]
N[25] addi 4 N[26, 0]
N[26] addi 4 N[27, 0]
N[27] addi 4 N[28, 0]
N[28] addi 4 N[29, 0]
N[29] addi 4 N[30, 0]
N[30] addi 4 N[31, 0]
N[31] addi 4 N[32, 0]
N[32] addi 4 N[33, 0]
N[33] addi 4 N[34, 0]
N[34] addi 4 N[35, 0]
N[35] addi 4 N[36, 0]
N[36] addi 4 N[37, 0]
N[37] addi 4 N[38, 0]
N[38] addi 4 N[39, 0]
N[39] addi 4 N[40, 0]
N[40] addi 4 N[41, 0]
N[41] addi 4 N[42, 0]
N[42] addi 4 N[43, 0]
N[43] addi 4 N[44, 0]
N[44] addi 4 N[45, 0]
N[45] addi 4 N[46, 0]
N[46] addi 4 N[47, 0]
N[47] addi 4 N[48, 0]
N[48] addi 4 N[49, 0]
N[49] addi 4 N[50, 0]
N[50] addi 4 N[51, 0]
N[51] addi 4 N[52, 0]
N[52] addi 4 N[53, 0]
N[53] addi 4 N[54, 0]
N[54] addi 4 N[55, 0]
N[55] addi 4 N[56, 0]
N[56] addi 4 N[57, 0]
N[57] addi 4 N[58, 0]
N[58] addi 4 N[59, 0]
N[59] addi 4 N[60, 0]
N[60] addi 4 N[61, 0]
N[61] addi 4 N[62, 0]
N[62] addi 4 N[63, 0]
N[63] addi 4 N[64, 0]
N[64] addi 4 N[65, 0]
N[65] addi 4 N[66, 0]
N[66] addi 4 N[67, 0]
N[67] addi 4 N[68, 0]
N[68] addi 4 N[69, 0]
N[69] addi 4 N[70, 0]
N[70] addi 4 N[71, 0]
N[71] addi 4 N[72, 0]
N[72] addi 4 N[73, 0]
N[73] addi 4 N[74, 0]
N[74] addi 4 N[75, 0]
N[75] addi 4 N[76, 0]
N[76] addi 4 N[77, 0]
N[77] addi 4 N[78, 0]
N[78] addi 4 N[79, 0]
N[79] addi 4 N[80, 0]
N[80] addi 4 N[81, 0]
N[81] addi 4 N[82, 0]
N[82] addi 4 N[83, 0]
N[83] addi 4 N[84, 0]
N[84] addi 4 N[85, 0]
N[85] addi 4 N[86, 0]
N[86] addi 4 N[87, 0]
N[87] addi 4 N[88, 0]
N[88] addi 4 N[89, 0]
N[89] addi 4 N[90, 0]
N[90] addi 4 N[91, 0]
N[91] addi 4 N[92, 0]
N[92] addi 4 N[93, 0]
N[93] addi 4 N[94, 0]
N[94] addi 4 N[95, 0]
N[95] addi 4 N[96, 0]
N[96] addi 4 N[97, 0]
N[97] addi 4 N[98, 0]
N[98] addi 4 N[99, 0]
N[99] addi 4 N[100, 0]
N[100] addi 4 N[101, 0]
N[101] addi 4 N[102, 0]
N[102] addi 4 N[103, 0]
N[103] addi 4 N[104, 0]
N[104] addi 4 N[105, 0]
N[105] addi 4 N[106, 0]
N[106] addi 4 N[107, 0]
N[107] addi 4 N[108, 0]
N[108] addi 4 N[109, 0]
N[109] addi 4 N[110, 0]
N[110] addi 4 N[111, 0]
N[111] addi 4 N[112, 0]
N[112] addi 4 N[113, 0]
N[113] addi 4 N[114, 0]
N[114] addi 4 N[115, 0]
N[115] addi 4 N[116, 0]
N[116] addi 4 N[117, 0]
N[117] addi 4 N[118, 0]
N[118] addi 4 N[119, 0]
N[119] addi 4 N[120, 0]
N[120] addi 4 N[121, 0]
N[121] addi 4 N[122, 0]
N[122] addi 4 N[123, 0]
N[123] addi 4 N[124, 0]
N[124] addi 4 N[125, 0]
N[125] addi 4 N[126, 0]
N[126] addi 4 W[3]
N[127] bro exit_block
W[3] write G[3]

.bend

.bbegin exit_block
N[0] movi SYS_EXIT W[0]
N[1] scall

W[0] write G[0]
.bend exit_block
