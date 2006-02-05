SB=/cygdrive/c/Program\ Files/SourceBoost
CC=$(SB)/boostc.pic18.exe
CFLAGS=-O0 -W1 -t 18F4620
LINK=$(SB)/boostlink.pic.exe
LIBS="C:\Program Files\SourceBoost\Lib\libc.pic18.lib" "C:\Program Files\SourceBoost\Lib\rand.pic18.lib"
LFLAGS=$(LIBS) -O1 -t 18F4620

intro.hex: gpu_pic.obj intro.obj main.obj spu_pic.obj
	$(LINK) $(LFLAGS) $^ -p $(@:.hex=)

gpu_pic.obj: gpu_pic.c gpu_pic.h common.h types.h
	$(CC) $(CFLAGS) $<

intro.obj: intro.c gpu_pic.h spu_pic.h intro.h common.h types.h
	$(CC) $(CFLAGS) $<

main.obj: main.c intro.h gpu_pic.h spu_pic.h
	$(CC) $(CFLAGS) $<

spu_pic.obj: spu_pic.c spu_pic.h
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.obj *.hex *.lst *.cof *.stat *.tree *.asm *.casm

.PHONY: clean
