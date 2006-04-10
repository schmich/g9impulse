paste: paste.tab.c
	g++ -o $@ $^

%.tab.c: %.y %.yy.c
	bison $<

%.yy.c: %.flex
	flex -o$@ $<

clean:
	rm -f *.tab.c *.yy.c paste.exe

.PRECIOUS: paste.yy.c
