; Test:     null_5
; Author:   Karu
; Purpose:  Tests null propogation via load

; Final result should be 0xc001

.set SYS_EXIT=1
.data
data_block_0:
.asciz "this is data"           
        
.text
.global _start
.bbegin _start
N[0] genu 0xc001 W[3]
N[127] bro foo
W[3] write G[3]

n[4] genu %lo(data_block_0) n[5]
n[5] app %bottom(data_block_0)  n[6]
n[6] mov w[0] w[1]
w[0] write g[0]
w[1] write g[1]                
.bend

.bbegin foo
N[0] null N[1,0]  
N[1] ld 0x0 N[2,0]
N[2] addi 0x4 W[3]
N[127] bro exit
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend

