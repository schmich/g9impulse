BINARY=starcell.hex

MCC_DIR=/cygdrive/c/MCC18
MCC_WIN_DIR=c:\MCC18

DIRS=$(shell find . -maxdepth 1 -type d)
MCC_INC=$(patsubst %,/i%,$(DIRS))
GCC_INC=$(patsubst %,-I%,$(DIRS))

CC=$(MCC_DIR)/bin/mcc18.exe
CFLAGS=-p=18F4620 /i"$(MCC_WIN_DIR)\h" $(MCC_INC) -k -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

LINK=$(MCC_DIR)/bin/mplink.exe
LIBS=/l"$(MCC_WIN_DIR)\lib" /k"$(MCC_WIN_DIR)\lkr" 18f4620.lkr
LFLAGS=$(LIBS)

DEPC=gcc
DEPFLAGS=-MM -MG -I$(MCC_DIR)/h $(GCC_INC) -D__18F4620

SRCS=$(shell find . -iname '*.c')
FLAT_SRCS=$(shell find . -iname '*.c' | ruby -p -e '$$_ = $$_[/[^\/]+\.c$$/] + "\n"')
OBJS=$(patsubst %,$(BUILD_DIR)/%,$(FLAT_SRCS:.c=.o))

BUILD_DIR=.build
DEPFILE=$(BUILD_DIR)/deps
OUT=$(BUILD_DIR)/$(BINARY)

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
	@echo -n "Linking $(shell basename $@)..."
	@$(LINK) $(LFLAGS) $^ /w /m"$(@:.hex=.map)" /o"$(@:.hex=.cof)" 2>&1 >/dev/null
	@cp $@ ../../assets
	@echo "done"

%.anim.inl: animations/%.anim
	@echo -n "Generating animation $@..."
	@scripts/anim.rb $< > $@
	@echo "done"

%.lvl.inl: levels/%.lvl
	@echo -n "Generating level $@..."
	@scripts/level.rb $< > $@
	@echo "done"

%.o:
	@echo -n "Compiling $(shell basename $(@:.o=.c))..."
	@$(CC) $(CFLAGS) $(shell find . -iname $(shell basename $@ .o).c) -fo=$@
	@echo "done"

ifeq '' '$(findstring clean,$(MAKECMDGOALS))'
    -include $(DEPFILE)
endif

$(DEPFILE):
	@echo -n "Generating dependencies..."
	@mkdir -p $(BUILD_DIR)
	@$(DEPC) $(DEPFLAGS) $(SRCS) > $(DEPFILE).tmp
	@cat $(DEPFILE).tmp | sed 's/^\(.*\.o\)/$(BUILD_DIR)\/\1/' > $(DEPFILE)
	@rm -f $(DEPFILE).tmp
	@echo "done"

clean:
	rm -f *.obj *.hex *.lst *.cof *.stat *.tree *.asm *.casm *.o *.map *.err *.anim.inl *.lvl.inl
	rm -rf .build

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
