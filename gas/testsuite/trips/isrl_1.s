
;;; Test :      isrl_1.s
;;; author :	 sibi
;;; purpose :	to test the integer srl  instruction
;;; result must be 0x19 


.set SYS_EXIT=1

.text
.global _start

.bbegin _start
N[0] movi 100 N[2,0]
N[1] movi 2 N[2,1]
N[2] srl W[3]
N[3] bro exit_block

W[3] write G[3]
.bend

.bbegin exit_block
N[0] movi SYS_EXIT W[0]
N[1] scall

W[0] write G[0]
.bend


