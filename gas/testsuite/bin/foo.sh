#!/bin/bash

srcdir=~/Projects/trips/tmax
target=bro
log=bro

${srcdir}/bin/tmax --total_num_x=4 --total_num_y=4 -config ${srcdir}/config/tmax_conf.cfg -bpredconfig ${srcdir}/config/bpred.2K.tile16 -s -o ${log} ${target}

