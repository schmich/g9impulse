OUT=starcell.hex
MCC_DIR=/cygdrive/c/MCC18
MCC_WIN_DIR=c:\MCC18
CC=$(MCC_DIR)/bin/mcc18.exe
CFLAGS=-p=18F4620 /i"$(MCC_WIN_DIR)\h" -k -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-
LINK=$(MCC_DIR)/bin/mplink.exe
LIBS=/l"$(MCC_WIN_DIR)\lib" /k"$(MCC_WIN_DIR)\lkr" 18f4620.lkr
LFLAGS=$(LIBS)

DEPC=gcc
DEPFLAGS=-MM -MG -I$(MCC_DIR)/h -D__18F4620

SRCS=$(shell find . -maxdepth 1 -iname '*.c')
OBJS=$(SRCS:.c=.o)
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
	@echo -n "Linking $(OUT)..."
	@$(LINK) $(LFLAGS) $^ /w /m"$(@:.hex=.map)" /o"$(@:.hex=.cof)"
	@cp $@ ../../assets
	@echo "done"

%.anim.inl: animations/%.anim scripts/anim.rb
	@echo -n "Generating animation $(patsubst animations/%,%,$<)..."
	@scripts/anim.rb $< > $@
	@echo "done"

%.lvl.inl: levels/%.lvl scripts.level.rb
	@echo -n "Generating level $(patsubst levels/%,%,$<)..."
	@scripts/level.rb $< > $@
	@echo "done"

%.o: %.c
	@echo -n "Compiling $<..."
	@$(CC) $(CFLAGS) $< -fo=$@
	@echo "done"

-include $(DEPFILE)

$(DEPFILE):
	@echo -n "Generating dependencies..."
	@$(DEPC) $(DEPFLAGS) $(SRCS) > $(DEPFILE)
	@echo "done"

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
