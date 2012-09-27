; Test:     sd_1
; Author:   Robert McDonald
; Purpose:  Tests a single store doubleword

; Final exit value should be 0xc00lcafec00lcafe
; Final testdata value should be 0xc00lcafec00lcafe
; Will need to rely upon trace check for mem testing

.set SYS_EXIT=1

.data
testdata:
.quad 0xdeadbeefdeadbeef

.text
.global _start
.bbegin _start
N[0] genu 0xc001 N[1]
N[1] app 0xcafe N[2]
N[2] app 0xc001 N[3]
N[3] app 0xcafe N[4]
N[4] mov N[7,1] W[3]
N[5] genu %lo(testdata) N[6]
N[6] app %bottom(testdata) N[7,0]
N[7] sd
N[127] bro exit
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend