OUT=starcell.hex
MCC_DIR=/cygdrive/c/MCC18
MCC_WIN_DIR=c:\MCC18
CC=$(MCC_DIR)/bin/mcc18.exe
CFLAGS=-p=18F4620 /i"$(MCC_WIN_DIR)\h" /i.. /isdk /ienemies /iprojectiles /ibehaviors /ilevels -k -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-
LINK=$(MCC_DIR)/bin/mplink.exe
LIBS=/l"$(MCC_WIN_DIR)\lib" /k"$(MCC_WIN_DIR)\lkr" 18f4620.lkr
LFLAGS=$(LIBS)

DEPC=gcc
DEPFLAGS=-MM -MG -I$(MCC_DIR)/h -Isdk -Ienemies -Iprojectiles -Ibehaviors -Ilevels -D__18F4620

SRCS=$(shell find . -iname '*.c')
OBJS0=$(SRCS:.c=.o)
OBJS1=$(patsubst ./sdk/%.o,./%.o,$(OBJS0))
OBJS2=$(patsubst ./enemies/%.o,./%.o,$(OBJS1))
OBJS3=$(patsubst ./projectiles/%.o,./%.o,$(OBJS2))
OBJS4=$(patsubst ./behaviors/%.o,./%.o,$(OBJS3))
OBJS=$(patsubst ./levels/%.o,./%.o,$(OBJS4))
DEPFILE=.deps

DEBUG=false

ifeq ($(MAKECMDGOALS),)
	DEBUG=true
endif

ifeq ($(MAKECMDGOALS),all)
	DEBUG=true
endif

ifeq ($(DEBUG),true)
	CFLAGS += -D_DEBUG
endif

all: $(DEPFILE) $(OUT)

release: all

$(OUT): $(OBJS)
	$(LINK) $(LFLAGS) $^ /w /m"$(@:.hex=.map)" /o"$(@:.hex=.cof)"
	cp $@ ../../assets

%.anim.inl: animations/%.anim
	scripts/anim.rb $< > $@

%.lvl.inl: levels/%.lvl
	scripts/level.rb $< > $@

%.o: %.c
	$(CC) $(CFLAGS) $< -fo=$@

%.o: sdk/%.c
	$(CC) $(CFLAGS) $< -fo=$@

%.o: enemies/%.c
	$(CC) $(CFLAGS) $< -fo=$@

%.o: projectiles/%.c
	$(CC) $(CFLAGS) $< -fo=$@

%.o: behaviors/%.c
	$(CC) $(CFLAGS) $< -fo=$@

%.o: levels/%.c
	$(CC) $(CFLAGS) $< -fo=$@

-include $(DEPFILE)

$(DEPFILE):
	$(DEPC) $(DEPFLAGS) $(SRCS) > $(DEPFILE)

clean:
	rm -f *.obj *.hex *.lst *.cof *.stat *.tree *.asm *.casm *.o *.map *.err *.anim.inl *.lvl.inl

depclean:
	rm -f $(DEPFILE)

realclean: clean depclean
superclean: realclean
ultraclean: realclean
megaclean: realclean
uberclean: realclean
crazyclean: realclean

.PHONY: all release clean depclean realclean \
	superclean ultraclean megaclean \
	uberclean crazyclean
