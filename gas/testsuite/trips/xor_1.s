;;; Test :	 xor_1
;;; Author :	 Jacob Leverich
;;; Purpose :	 To test the logical XOR instruction
;;; Final Result must be 0x55

.set SYS_EXIT=1

.text
.global _start

.bbegin _start
N[0] movi 0xFF N[2,0]
N[1] movi 0xAA N[2,1]
N[2] xor W[3]
N[3] bro exit_block

W[3] write G[3]
.bend _start

.bbegin exit_block
N[0] movi SYS_EXIT W[0]
N[1] scall

W[0] write G[0]
.bend exit_block
