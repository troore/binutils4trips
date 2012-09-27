; Test:     itlb_miss_1
; Author:   Robert McDonald
; Purpose:  Test detection of an itlb miss

; Final result should be a Fetch Exception

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] movi 0x1 N[1]
N[1] slli 32 N[127]  ; make addr 0x0100000000
N[127] br
.bend
