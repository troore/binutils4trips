srcdir = ~/Projects/trips/ttools2
source = bro.o
target = bro

$(target) : $(source)
	$(srcdir)/sbin/tcc -O4 -Ws,'-c 4' -Ws,'-r 4' -Ws,'-topology tmax' -msize=8 $(source) -o $(target) -st 
