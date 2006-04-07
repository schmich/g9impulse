BINARY=starcell.hex

MCC_DIR=/cygdrive/c/MCC18
MCC_WIN_DIR=c:\MCC18

DIRS=$(shell find . -maxdepth 1 -type d)
MCC_INC=$(patsubst %,/i%,$(DIRS))
GCC_INC=$(patsubst %,-I%,$(DIRS))

CC=$(MCC_DIR)/bin/mcc18.exe
CFLAGS=-p=18F4620 /i"$(MCC_WIN_DIR)\h" $(MCC_INC) -k

LINK=$(MCC_DIR)/bin/mplink.exe
LIBS=/l"$(MCC_WIN_DIR)\lib" /k"$(MCC_WIN_DIR)\lkr" sdk/18f4620.lkr
LFLAGS=$(LIBS)

TRY=tools/try
FIXDEPS=tools/fixdeps.rb

DEPC=gcc
DEPFLAGS=-MM -MG -I$(MCC_DIR)/h $(GCC_INC) -D__18F4620

BUILD_DIR=
BUILD_DEBUG_DIR=.debug
BUILD_RELEASE_DIR=.release

DEBUG=false

ifeq ($(MAKECMDGOALS),)
	DEBUG=true
endif

ifeq ($(MAKECMDGOALS),all)
	DEBUG=true
endif

ifeq ($(MAKECMDGOALS),depclean)
	DEBUG=true
endif

ifeq ($(DEBUG),true)
	CFLAGS += -D_DEBUG=1
	BUILD_DIR=$(BUILD_DEBUG_DIR)
else
	BUILD_DIR=$(BUILD_RELEASE_DIR)
endif

DEPFILE=$(BUILD_DIR)/deps
OUT=$(BUILD_DIR)/$(BINARY)

SRCS=$(shell find . -maxdepth 2 -iname '*.c')
FLAT_SRCS=$(shell find . -maxdepth 2 -iname '*.c' | ruby -p -e '$$_ = $$_[/[^\/]+\.c$$/] + "\n"')
OBJS=$(patsubst %,$(BUILD_DIR)/%,$(FLAT_SRCS:.c=.o))

all: $(DEPFILE) $(OUT)

release: all

$(OUT): $(OBJS)
	@echo -n "Linking $(shell basename $@)..."
	@$(TRY) $(LINK) $(LFLAGS) $^ /w /m"$(@:.hex=.map)" /o"$(@:.hex=.cof)"
	@cp $@ ../../assets
	@echo "OK"

$(BUILD_DIR)/%.anim.inl: animations/%.anim tools/anim.rb
	@echo -n "Generating animation $(shell basename $@ .inl)..."
	@tools/anim.rb $< > $@
	@echo "OK"

$(BUILD_DIR)/%.lvl.inl: levels/%.lvl tools/level.rb
	@echo -n "Generating level $(shell basename $@ .inl)..."
	@tools/level.rb $< > $@
	@echo "OK"

%.o:
	@echo -n "Compiling $(shell basename $(@:.o=.c))..."
	@$(TRY) $(CC) $(CFLAGS) $(shell find . -iname $(shell basename $@ .o).c) -fo=$@
	@echo "OK"

ifeq '' '$(findstring clean,$(MAKECMDGOALS))'
    ifeq '' '$(findstring nuke,$(MAKECMDGOALS))'
        -include $(DEPFILE)
    endif
endif

$(DEPFILE):
	@echo -n "Generating dependencies..."
	@mkdir -p $(BUILD_DIR)
	@$(DEPC) $(DEPFLAGS) $(SRCS) > $(DEPFILE).tmp
	@cat $(DEPFILE).tmp | $(FIXDEPS) $(BUILD_DIR) > $(DEPFILE)
	@rm -f $(DEPFILE).tmp
	@echo "OK"

clean:
	rm -f *.obj *.hex *.lst *.cof *.stat *.tree *.asm *.casm *.o *.map *.err *.anim.inl *.lvl.inl

depclean:
	rm -f $(DEPFILE)

release-depclean: depclean

nuke: depclean clean
	rm -rf $(BUILD_DEBUG_DIR)

release-nuke: depclean clean
	rm -rf $(BUILD_RELEASE_DIR)

.PHONY: all release clean depclean nuke
