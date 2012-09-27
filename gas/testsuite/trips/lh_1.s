; Test:     lh_1
; Author:   Robert McDonald
; Purpose:  Tests a single load halfword

; Final result should be 0xf00d

.set SYS_EXIT=1

.data
testdata:
.quad 0xcafebabefeedf00d

.text
.global _start
.bbegin _start
N[0] genu %lo(testdata) N[1]
N[1] app %bottom(testdata) N[2]
N[2] lh 6 W[3]
N[127] bro exit
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
