; Test:     ctrl_complex_2
; Author:   Ramadass Nagarajan
; Purpose:  Test a loop sequence with ctrl changes using only loads and stores

; Final result should be 0x2e5bb07e3dafeb45

.set SYS_EXIT=1

.data

;; define memory locations for some variables
sum:
	.quad 0x0
index:	
	.quad 0x10
num:	
	.quad 0x100

.text
.global _start

;;  set loop count to 100
.bbegin _start

N[0] movi 100 N[3,1]
N[1] genu %lo(num) N[2]
N[2] app %bottom(num) N[3,0]
N[3] sd	0

N[4] movi 0 N[7,1]
N[5] genu %lo(index) N[6]
N[6] app %bottom(index) N[7,0]
N[7] sd	0

N[8] movi 0 N[11,1]
N[9] genu %lo(sum) N[10]
N[10] app %bottom(sum) N[11,0]
N[11] sd 0
	
N[12] bro prologue
	
.bend

;; loop body starts
	
.bbegin prologue

;; read loop index
N[0] genu %lo(index) N[1]
N[1] app %bottom(index) N[2]
N[2] ld 0 N[3]

N[3] andi 1 N[4] 
N[4] teqi 1 N[5]
N[5] mov N[6,P] N[7,P]	
N[6] bro_t addblock
N[7] bro_f mulblock		

.bend

.bbegin addblock

;;  read partial sum
N[0] genu %lo(sum) N[1]
N[1] app %bottom(sum) N[2]
N[2] mov N[3] N[9,0]
N[3] ld 0 N[8,0]

;; read index
N[4] genu %lo(index) N[5]
N[5] app %bottom(index) N[6]
N[6] mov N[7]
N[7] ld 0 N[8,1]

N[8] add N[9,1]

N[9] sd 0 
N[10] bro epilogue
							
.bend

.bbegin mulblock

;;  read partial sum
N[0] genu %lo(sum) N[1]
N[1] app %bottom(sum) N[2]
N[2] mov N[3] N[9,0]
N[3] ld 0 N[8,0]

;; read index
N[4] genu %lo(index) N[5]
N[5] app %bottom(index) N[6]
N[6] mov N[7]
N[7] ld 0 N[8,1]

N[8] mul N[9,1]

N[9] sd 0 
N[10] bro epilogue

.bend 

.bbegin epilogue

;;  read loop count
N[0] genu %lo(num) N[1]
N[1] app %bottom(num) N[2]
N[2] ld 0 N[10,0]
		
;; read index
N[4] genu %lo(index) N[5]
N[5] app %bottom(index) N[6]
N[6] mov N[7] N[13,0]
N[7] ld 0 N[8]	

;;  add 1 to loop index
N[8] addi 1 N[9]
N[9] mov N[10,1] N[13,1]
	
;;  test if loop count is reached
N[10] teq N[11,P] N[12,P]
			
N[11] bro_f prologue
N[12] bro_t exit
			
;;  write loop index
N[13] sd 0

.bend

;;  return sum in G[4]
.bbegin exit
N[4] genu %lo(sum) N[5]
N[5] app %bottom(sum) N[6]
N[6] mov N[7]
N[7] ld 0 W[3]

N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
W[3] write G[3]

.bend
