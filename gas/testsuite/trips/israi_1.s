
;;; Test :      israi_1.s
;;; author :	 sibi
;;; purpose :	to test the integer srai  instruction
;;; result must be 0xfffffffffffffffb 


.set SYS_EXIT=1

.text
.global _start

.bbegin _start
N[0] movi -10 N[1,0]
N[1] srai 1 W[3]
N[3] bro exit_block

W[3] write G[3]
.bend

.bbegin exit_block
N[0] movi SYS_EXIT W[0]
N[1] scall

W[0] write G[0]
.bend


