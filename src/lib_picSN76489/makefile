PROJ_NAME = lib76489.a
SRCDIR = src
SOURCE = $(wildcard $(SRCDIR)/*.c)
HEADER = $(notdir $(SOURCE:.c=.h))
OBJDIR = obj
OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(SOURCE:.c=.p1)))
OUT = $(PROJ_NAME)
TESTDIR = test
TESTSRC = $(wildcard $(TESTDIR)/*.c)
TESTOUT = $(TESTDIR)/out/
TEST = $(addsuffix .hex, $(addprefix $(TESTOUT), $(basename $(notdir $(TESTSRC)))))
DOXYGEN_GEN = doxygen
DOXYGEN_CFG = dox.cfg
MCPU = 18F45K50

CC = xc8-cc
AR = xc8-ar
CFLAGS = -I. -O2 -ansi -xassembler-with-cpp -Wa,-a -mcpu=$(MCPU)
LFLAGS = -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -fno-short-double -fno-short-float -ginhx032 -I. -l$(OUT) -msummary=-psect,-class,+mem,-hex,-file
ARFLAGS = -r

.PHONY: clean dox_gen

all: $(OUT) $(TEST) dox_gen

$(TEST): $(TESTSRC) $(OUT)
	mkdir -p $(TESTOUT)
	$(CC) $(CFLAGS) $< -o $@ $(LFLAGS)

$(OUT): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $<

$(OBJDIR)/%.p1: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
	
dox_gen: 
	$(DOXYGEN_GEN) $(DOXYGEN_CFG) $(HEADER)

clean:
	rm -rf $(OUT) $(OBJDIR) $(TESTOUT) $(DOXYGEN_GEN)
