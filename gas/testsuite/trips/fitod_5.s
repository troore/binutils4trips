; Test:     fitod_5
; Author:   Karu
; Purpose:  Test generates all 7...Fs and converts to double

; Final result should be 0x43E0000000000000
; 9223372036854775808.000000
; 4890909195324358656 int


.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] genu 0x7FFF N[1]
N[1] app 0xFFFF N[2]
N[2] app 0xFFFF N[3]
N[3] app 0xFFFF N[4]     
N[4] fitod W[3] 
N[5] bro exit
       
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend
