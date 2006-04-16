paste: paste.tab.c paste.yy.o
	g++ -o $@ $^

paste.yy.o: paste.yy.c
	g++ -c $< -o $@

paste.tab.c paste.tab.h: paste.y paste.l
	bison -d $<

paste.yy.c: paste.l paste.tab.h
	flex -o$@ $<

clean:
	rm -f *.tab.c *.tab.h *.yy.c *.o paste.exe

.PRECIOUS: paste.yy.c
