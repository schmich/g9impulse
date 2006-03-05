BINARY=starcell.hex

MCC_DIR=/cygdrive/c/MCC18
MCC_WIN_DIR=c:\MCC18

DIRS=$(shell find . -maxdepth 1 -type d)
MCC_INC=$(patsubst %,/i%,$(DIRS))
GCC_INC=$(patsubst %,-I%,$(DIRS))

CC=$(MCC_DIR)/bin/mcc18.exe
CFLAGS=-p=18F4620 /i"$(MCC_WIN_DIR)\h" $(MCC_INC) -k -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

LINK=$(MCC_DIR)/bin/mplink.exe
LIBS=/l"$(MCC_WIN_DIR)\lib" /k"$(MCC_WIN_DIR)\lkr" sdk/18f4620.lkr
LFLAGS=$(LIBS)

TRY=tools/try
FIXDEPS=tools/fixdeps.rb

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
	@$(TRY) $(LINK) $(LFLAGS) $^ /w /m"$(@:.hex=.map)" /o"$(@:.hex=.cof)"
	@cp $@ ../../assets
	@echo "done"

$(BUILD_DIR)/%.anim.inl: animations/%.anim tools/anim.rb
	@echo -n "Generating animation $(shell basename $@)..."
	@$(TRY) tools/anim.rb $< > $@
	@echo "done"

$(BUILD_DIR)/%.lvl.inl: levels/%.lvl tools/level.rb
	@echo -n "Generating level $(shell basename $@)..."
	@$(TRY) tools/level.rb $< > $@
	@echo "done"

%.o:
	@echo -n "Compiling $(shell basename $(@:.o=.c))..."
	@$(TRY) $(CC) $(CFLAGS) $(shell find . -iname $(shell basename $@ .o).c) -fo=$@
	@echo "done"

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
	@echo "done"

clean:
	rm -f *.obj *.hex *.lst *.cof *.stat *.tree *.asm *.casm *.o *.map *.err *.anim.inl *.lvl.inl
	rm -rf .build

depclean:
	rm -f $(DEPFILE)

nuke: depclean clean

.PHONY: all release clean depclean nuke
