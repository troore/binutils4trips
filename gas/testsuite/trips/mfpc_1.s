;;; Test    :  mfpc_1
;;; Author  :  Jacob Leverich
;;; Purpose :  To test the Move From PC instruction
;;; Final Result must be 0x1

.set SYS_EXIT=1

.text
.global _start

.bbegin _start
N[0] mfpc 0 N[3,0]
N[1] genu %lo(_start) N[2]
N[2] app %bottom(_start) N[3,1]
N[3] teq W[3]
N[4] bro exit_block
W[3] write G[3]
.bend _start

.bbegin exit_block
N[0] movi SYS_EXIT W[0]
N[1] scall

W[0] write G[0]
.bend exit_block
