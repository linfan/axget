################################
##      Makefile for Axget    ##
##                            ##
## Copyright 2001-2012 Lintux ##
## Copyright 2013      Freyr  ##
################################

### DEFINITIONS

-include ../Makefile.settings

OBJDIR = obj

all: prepare $(OUTFILE)

prepare:
ifneq ($(OBJDIR), $(wildcard $(OBJDIR)))
	mkdir -p "$(OBJDIR)"
	mkdir -p "$(OBJDIR)/lib"
endif

clean:
	rm -fr $(OUTFILE) $(OBJDIR) core.*

### MAIN PROGRAM

SRC = $(wildcard lib/*.c) $(wildcard *.c)

OBJS = $(SRC:%.c=%.o)

OBJFILES = $(OBJS:%=$(OBJDIR)/%)
$(OUTFILE): $(OBJFILES) $(OBJDIR)/main.o
	$(CC) $^ -o $(OUTFILE) $(LFLAGS)
	$(STRIP) $(OUTFILE)

$(OBJDIR)/%.o: %.c
	$(CC) -c $*.c -o $(OBJDIR)/$*.o $(CFLAGS) -fPIC

LIBOUTFILE = lib$(OUTFILE).so

unittest: $(OBJS)
	$(CC) $^ --share -o $(LIBOUTFILE) $(LFLAGS)
	mv $(LIBOUTFILE) ../unittest/lib/

ut: unittest
	$(MAKE) -C ../unittest

install-bin:
	mkdir -p $(DESTDIR)$(BINDIR)/
	cp $(OUTFILE) $(DESTDIR)$(BINDIR)/$(OUTFILE)

uninstall-bin:
	rm -f $(BINDIR)/$(OUTFILE)

tags:
	ctags --languages=c --fields=+SKz --sort=foldcase *.c

