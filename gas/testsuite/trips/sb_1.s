; Test:     sb_1
; Author:   Robert McDonald
; Purpose:  Tests a single store byte

; Final exit value should be 0x1234
; Final testdata value should be 0x0034000000000000
; Will need to rely upon trace check for mem testing

.set SYS_EXIT=1

.data
testdata:
.quad 0x0000000000000000

.text
.global _start
.bbegin _start
N[0] genu 0x1234 N[1]
N[1] mov N[4,1] W[3]
N[2] genu %lo(testdata) N[3]
N[3] app %bottom(testdata) N[4,0]
N[4] sb 1
N[127] bro exit
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
