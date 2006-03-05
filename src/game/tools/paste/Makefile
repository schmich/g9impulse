OUT=paste

GRAMMAR=$(shell /bin/ls -1 *.y | head -n 1)
GRAMMAR_SRC=$(GRAMMAR:.y=.hs)
SRCS=$(shell /bin/ls -1 *.hs)
SRCS+=$(GRAMMAR_SRC)
OBJS=$(SRCS:.hs=.o)

all: $(OUT)

$(OUT): $(OBJS)
	ghc -o $@ $^

%.o: %.hs
	ghc -c $<

%.hs: %.y
	happy $<

clean:
	rm -f level.hs parse *.o *.hi $(GRAMMAR_SRC) $(OUT)

.PRECIOUS: $(GRAMMAR_SRC)
