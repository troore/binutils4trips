; Test    : break_1
; Author  : Robert
; Purpose : Test breakpoint exception (break_before)

; Final result should be an exception

.set SYS_EXIT = 1
.set BREAK_BEFORE = 0x10

.text
.global _start

.bbegin _start BREAK_BEFORE
N[0] movi 0xFF N[2,0]
N[1] movi 0x55 N[2,1]
N[2] and W[3]
N[3] bro exit_block
W[3] write G[3]
.bend _start

.bbegin exit_block
N[0] movi SYS_EXIT W[0]
N[1] scall

W[0] write G[0]
.bend exit_block
