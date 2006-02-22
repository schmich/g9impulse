MCC_DIR=/cygdrive/c/MCC18
MCC_WIN_DIR=c:\MCC18
CC=$(MCC_DIR)/bin/mcc18.exe
CFLAGS=-p=18F4620 /i"$(MCC_WIN_DIR)\h" -k -mL -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- -D_DEBUG
LINK=$(MCC_DIR)/bin/mplink.exe
LIBS=/l"$(MCC_WIN_DIR)\lib" /k"$(MCC_WIN_DIR)\lkr" 18f4620.lkr
LFLAGS=$(LIBS)

starcell.hex: gpu_pic.o intro.o main.o spu_pic.o input.o delay.o delay.o input.o entity.o player.o image.o sprite.o memory.o sralloc.o f16.o updateable.o behavior.o background.o destroyable.o animation.o
	$(LINK) $(LFLAGS) $^ /w /m"$(@:.hex=.map)" /o"$(@:.hex=.cof)"
	cp $@ ../../assets

animation.o: animation.c animation.h destroyable.h
	$(CC) $(CFLAGS) $< -fo=$@

destroyable.o: destroyable.c destroyable.h
	$(CC) $(CFLAGS) $< -fo=$@

background.o: background.c background.h updateable.h
	$(CC) $(CFLAGS) $< -fo=$@

behavior.o: behavior.c behavior.h
	$(CC) $(CFLAGS) $< -fo=$@

updateable.o: updateable.c updateable.h entity.h
	$(CC) $(CFLAGS) $< -fo=$@

enemy.o: enemy.c enemy.h updateable.h
	$(CC) $(CFLAGS) $< -fo=$@

f16.o: f16.c f16.h enemy.h
	$(CC) $(CFLAGS) $< -fo=$@

sralloc.o: sralloc.c sralloc.h
	$(CC) $(CFLAGS) $< -fo=$@

memory.o: memory.c memory.h
	$(CC) $(CFLAGS) $< -fo=$@

player.o: player.c player.h entity.h
	$(CC) $(CFLAGS) $< -fo=$@

entity.o: entity.c entity.h common.h gpu_pic.h animation.h
	$(CC) $(CFLAGS) $< -fo=$@

image.o: image.c image.h gpu_pic.h
	$(CC) $(CFLAGS) $< -fo=$@

sprite.o: sprite.c sprite.h image.h gpu_pic.h
	$(CC) $(CFLAGS) $< -fo=$@

gpu_pic.o: gpu_pic.c gpu_pic.h common.h types.h
	$(CC) $(CFLAGS) $< -fo=$@

intro.o: intro.c gpu_pic.h spu_pic.h intro.h common.h types.h input.h delay.h
	$(CC) $(CFLAGS) $< -fo=$@

main.o: main.c intro.h gpu_pic.h spu_pic.h input.h delay.h
	$(CC) $(CFLAGS) $< -fo=$@

spu_pic.o: spu_pic.c spu_pic.h
	$(CC) $(CFLAGS) $< -fo=$@

delay.o: delay.c delay.h
	$(CC) $(CFLAGS) $< -fo=$@

input.o: input.c input.h
	$(CC) $(CFLAGS) $< -fo=$@

clean:
	rm -f *.obj *.hex *.lst *.cof *.stat *.tree *.asm *.casm *.o *.map

.PHONY: clean
