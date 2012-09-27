.data
_v0:
.ascii "hh"

.text
.bbegin foo$1
R[1] read G[1] N[1,0]
R[2] read G[2] N[8,0]

N[8] mov4 N[2,0] N[2,0] N[2,0] N[2,0]
N[1] addi N[2,1]
N[2] add W[3]

N[3] bro B[0]; foo$2

W[3] write G[3]
.bend
