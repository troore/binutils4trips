; Test:     null_1
; Author:   Robert McDonald
; Purpose:  Tests unconditional write nullification

; Final result should be 0xc001

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] genu 0xc001 W[3]
N[127] bro foo
W[3] write G[3]
.bend

.bbegin foo
N[0] null W[3]  ; this unconditional null is nonsensical, but should work
N[127] bro exit
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
