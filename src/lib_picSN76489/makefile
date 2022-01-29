PROJ_NAME = lib76489.a
SRCDIR = src
SOURCE = $(wildcard $(SRCDIR)/*.c)
OBJDIR = obj
OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(SOURCE:.c=.p1)))
OUTDIR = lib
OUT = $(addprefix $(OUTDIR)/, $(PROJ_NAME))
TESTDIR = test
TESTSRC = $(wildcard $(TESTDIR)/*.c)
TESTOUT = $(TESTDIR)/out/
TEST = $(addsuffix .hex, $(addprefix $(TESTOUT), $(basename $(notdir $(TESTSRC)))))
MCPU = 18F45K50

CC = xc8-cc
AR = xc8-ar
CFLAGS = -ansi -xassembler-with-cpp -Wa,-a -mcpu=$(MCPU)
LFLAGS = -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -fno-short-double -fno-short-float  -ginhx032 -I$(SRCDIR)/ -l$(OUT) -msummary=-psect,-class,+mem,-hex,-file
ARFLAGS = -r

.PHONY: clean

all: $(OUT) $(TEST)

$(TEST): $(TESTSRC)
	mkdir -p $(TESTOUT)
	$(CC) $(CFLAGS) $< -o $@ $(LFLAGS)

$(OUT): $(OBJECTS)
	mkdir -p $(OUTDIR)
	$(AR) $(ARFLAGS) $@ $<

$(OBJDIR)/%.p1: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OUTDIR) $(OBJDIR) $(TESTOUT)
