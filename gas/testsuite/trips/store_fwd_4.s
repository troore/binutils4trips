; Test:     store_fwd_4
; Author:   Robert McDonald
; Purpose:  Test advanced store forwarding within a block (sb,sb -> lh)

; Final result should be 0x1234

.set SYS_EXIT=1

.data
testdata:
.quad 0x0000000000000000

.text
.global _start
.bbegin _start
N[0] genu 0x12 N[64,1]
N[1] genu 0x34 N[65,1]

N[4] genu %lo(testdata) N[5]
N[5] app %bottom(testdata) N[6]
N[6] mov N[64] N[7]
N[7] mov N[65] N[66]

N[64] sb
N[65] sb 1
N[66] lh W[3]

N[127] bro exit
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
