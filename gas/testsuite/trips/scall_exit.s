; Test:     scall_exit
; Author:   Robert McDonald
; Purpose:  Tests the "exit" system call

; The exit value should be zero

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] movi SYS_EXIT W[0]
N[1] movi 0 W[3]
N[2] scall
W[0] write G[0]
W[3] write G[3]
.bend
