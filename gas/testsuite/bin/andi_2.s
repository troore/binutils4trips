;;; Test :	 andi_2
;;; Purpose :	 To test the logical AND immediate instruction
;;; Final Result must be 0x55

.set SYS_EXIT=1
.data
   string1: .asciz "Hello world!"

       .text
.global _start

.bbegin _start ;the start
N[0] movi 0xFF N[2,0]
N[2] andi 0x55 W[3]
N[3] bro exit_block

W[3] write G[3]
.bend _start

.bbegin exit_block
N[0] movi SYS_EXIT W[0]
N[1] scall

W[0] write G[0]
.bend exit_block

