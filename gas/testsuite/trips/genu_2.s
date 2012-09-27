; Test:     genu_2
; Author:   Robert McDonald
; Purpose:  Tests generate unsigned 32-bit constant

; Final result should be 0xfaded (1027565)

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] genu 0xf N[1]
N[1] app 0xaded W[3]
N[2] bro exit
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
