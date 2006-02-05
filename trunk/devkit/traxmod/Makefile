all:	traxmod

traxmod: traxmod.o mod.o cmd.o pattern.o row.o
	g++ traxmod.o mod.o cmd.o pattern.o row.o -o traxmod

clean:
	rm -f *.o *~ traxmod *#

