; Test:     lpf
; Author:   Yoder, stolen from McDonald's LB work
; Purpose:  Tests a single prefetch pseudo-insn

; Final result should be 0x99

.set SYS_EXIT=1
.set EXIT_VAL=0x99

.data
testdata:
.quad 0xcafebabefeedf00d
; virtual address
mem_addr:
.quad 0x080444480

.text
.global _start
.bbegin _start

N[2] genu %lo(mem_addr) N[3]
N[3] app %bottom(mem_addr) N[33,0]

N[0] genu %lo(testdata) N[1]
N[1] app %bottom(testdata) N[33,1]

N[33] sd S[8]

N[40] genu %lo(mem_addr) N[41]
N[41] app %bottom(mem_addr) N[53,0]

N[53] lpf L[6] 0xab

N[60] genu %lo(testdata) N[61]
N[61] app %bottom(testdata) N[63,1]

N[62] genu %lo(mem_addr) N[66]
N[66] app %bottom(mem_addr) N[63,0]

N[63] sb S[3]


N[127] bro exit
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[2] movi EXIT_VAL W[3]
N[11] scall
W[0] write G[0]
W[3] write G[3]
.bend
