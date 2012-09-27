; Test:     scall_1
; Author:   Karu
; Purpose:  Tests the system calls in a loop
;           Call write and brk in a loop
;           The write prints a string on stdout, (fd 0)
;           brk is called with brk(0) which is equivalent to not
;           doing anyting

; The exit value should be zero


.set SYS_EXIT=1
.set SYS_WRITE=4
.set SYS_BRK=45        

.data
string:
.asciz "Hello world\n"

.set STRING_SIZE=12
.text
        
.global _start

.bbegin _start
n[0] movi 10 W[0]
n[1] bro write_call
w[0] write G[8]     ; maintain loop count in G[8]
.bend

.bbegin write_call
N[0] genu %lo(string) N[1]
N[1] app %bottom(string) N[3]
N[2] movi 1 W[3]                ;  write to fd 1 == stdout
N[3] mov W[4]        
N[4] movi STRING_SIZE W[5]
N[5] scall
N[6] genu %lo(brk_call) n[7]
N[7] app %bottom(brk_call) n[8]
n[8] mov W[6]
n[9] movi SYS_WRITE W[0]
W[0] write G[0]
W[3] write G[3]
W[4] write G[4]
W[5] write G[5]
W[6] write G[2]             ; return address
.bend

.bbegin brk_call
N[1] movi 0 N[2]   ; brk(0) == do nothing
N[2] mov W[3]

N[5] scall
N[6] genu %lo(loop_tail) n[7]
N[7] app %bottom(loop_tail) n[8]
n[8] mov W[6]  
n[9] movi SYS_BRK W[0]   
W[0] write G[0]
W[3] write G[3]
W[6] write G[2]             ; return address
.bend

.bbegin loop_tail
R[0] read G[8] N[0]
N[0] addi -1 N[1]
N[1] mov N[2] W[0]
N[2] teqi 0 N[3]
N[3] mov N[4,p] N[5,p]
N[4] bro_t end
N[5] bro_f write_call
W[0] write G[8]
.bend loop_tail

.bbegin end
N[0] movi SYS_EXIT W[0]
N[1] movi 0 W[3]
N[2] scall
W[0] write G[0]
W[3] write G[3]
.bend
