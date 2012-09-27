srcdir = ~/Projects/trips/ttools2
source = bro_1.s
object = bro.o
target = bro

$(target) : $(object)
#	$(srcdir)/sbin/tcc -O4 -Ws,'-c 4' -Ws,'-r 4' -Ws,'-topology tmax' -msize=8 $(source) -o $(target) -st 
	../ref/tld -o $(target) $(object)
$(object) : $(source)
	./tas -o $(object) $(source)
clean:
	rm -rf `ls | grep '$(target)' | grep -v '\.s$$'`
