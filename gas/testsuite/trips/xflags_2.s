; Test:     xflags_2
; Author:   Robert McDonald
; Purpose:  Tests with various block header xflags

; Final result should be 0xf

.set SYS_EXIT=1
.set SYS_BRK=45

.data
testdata:
.quad 0x0000000000000000

.text
.global _start
.bbegin _start
N[0] genu %lo(testdata) N[1]
N[1] app %bottom(testdata) W[1]
W[1] write G[1]
N[127] bro foo1
.bend

.bbegin foo1 0x1
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(foo2) N[11]
N[11] app %bottom(foo2) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin foo2 0x2
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(foo3) N[11]
N[11] app %bottom(foo3) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin foo3 0x3
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(foo4) N[11]
N[11] app %bottom(foo4) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin foo4 0x4
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(foo5) N[11]
N[11] app %bottom(foo5) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin foo5 0x5
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(foo6) N[11]
N[11] app %bottom(foo6) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin foo6 0x6
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(foo7) N[11]
N[11] app %bottom(foo7) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin foo7 0x7
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(foo8) N[11]
N[11] app %bottom(foo8) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin foo8 0x8
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(foo9) N[11]
N[11] app %bottom(foo9) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin foo9 0x9
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(foo10) N[11]
N[11] app %bottom(foo10) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin foo10 0xA
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(foo11) N[11]
N[11] app %bottom(foo11) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin foo11 0xB
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(foo12) N[11]
N[11] app %bottom(foo12) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin foo12 0xC
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(foo13) N[11]
N[11] app %bottom(foo13) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin foo13 0xD
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(foo14) N[11]
N[11] app %bottom(foo14) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin foo14 0xE
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(foo15) N[11]
N[11] app %bottom(foo15) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin foo15 0xF
R[1] read G[1] N[0] N[2]
N[0] lb N[1]
N[1] addi 1 N[2,1]
N[2] sb
N[8] movi SYS_BRK W[0]
W[0] write G[0]
N[10] genu %lo(winddown) N[11]
N[11] app %bottom(winddown) W[2]
W[2] write G[2]
N[12] movi 0 W[3]
W[3] write G[3]
N[9] scall
.bend

.bbegin winddown
R[1] read G[1] N[0]
N[0] lb W[3]
W[3] write G[3]
N[127] bro exit
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
