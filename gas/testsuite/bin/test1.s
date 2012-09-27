.text
.bbegin foo$1
R[3] read G[3] N[2,0]
R[4] read G[4] N[2,1] N[1,0]

N[2] mul N[3,0] W[3]
N[1] addi 7 W[4]
N[3] addi 5 W[5]
N[0] bro foo$1

W[3] write G[3]
W[4] write G[4]
W[5] write G[5]
.bend
