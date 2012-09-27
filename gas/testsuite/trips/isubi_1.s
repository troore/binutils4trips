
;;; Test : isubi_1.s
;;; author :	 sibi
;;; purpose :	 to test the integer subi instruction
;;; final result must be 0x10

.set SYS_EXIT=1

.text
.global _start

.bbegin _start

N[0] movi 10 N[2,0]
N[1] movi 22 N[2,1]
N[2] add N[3]
N[3] subi 16 W[3]
N[4] bro exit_block

W[3] write G[3]
.bend

.bbegin exit_block
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend


