
;;; Test :      idivui_1.s
;;; author :	 sibi
;;; purpose :	to test the integer divui (u - unsigned,i-immediate) instruction
;;; result must be 0xe


.set SYS_EXIT=1

.text
.global _start

.bbegin _start
N[0] genu 1000 N[2,0]
N[1] genu 400 N[2,1]
N[2] add N[3,0]
N[3] divui 100 W[3]
N[4] bro exit_block

W[3] write G[3]
.bend

.bbegin exit_block
N[0] movi SYS_EXIT W[0]
N[1] scall

W[0] write G[0]
.bend


