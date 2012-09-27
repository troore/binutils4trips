; Test :       exec_test_2
; Author :     Karu
; Purpose :    To check sequencing of unpipelined divides on a single Etile
; Final Result must be 0x91

.set SYS_EXIT=1
        
.text
.global _start

.bbegin _start
  N[0] gens 440 N[16, 0]          ;  
  N[4] gens 330 N[20, 0]          ;  
  N[8] gens 270 N[24, 0]
  N[12] gens 130 N[28, 0]
  N[16] divsi 5 N[32, 0]        ;  result = 88
  N[20] divsi 10 N[32, 1]       ;  result = 33
  N[24] divsi 15 N[36, 0]       ;  result = 18
  N[28] divsi 20 N[36, 1]       ;  result = 6
  N[32] add N[40,0]             ;  result = 121
  N[36] add N[40,1]             ;  result = 24
  N[40] add W[3]                ;  result = 145
  N[1] bro exit_block
  W[3] write G[3]
.bend

.bbegin exit_block
N[0] movi SYS_EXIT W[0]
N[1] scall

W[0] write G[0]
.bend exit_block
