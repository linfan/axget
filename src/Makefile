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
	mkdir -p "$(OBJDIR)"

clean:
	rm -fr *.o $(OUTFILE) $(OBJDIR) search core

### MAIN PROGRAM

OBJS = axel.o conf.o conn.o ftp.o http.o search.o tcp.o

$(OUTFILE): $(OBJS) main.o
	$(CC) $^ -o $(OUTFILE) $(LFLAGS)
	$(STRIP) $(OUTFILE)

.c.o:
	$(CC) -c $*.c -o $*.o $(CFLAGS) -fPIC

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

