################################
##      Makefile for Axget    ##
##                            ##
## Copyright 2001-2012 Lintux ##
## Copyright 2013      Freyr  ##
################################

### DEFINITIONS

-include Makefile.settings

all: $(OUTFILE)
ifdef I18N
	$(MAKE) all -C i18n
endif

install: install-bin
	$(MAKE) install -C share
ifdef I18N
	$(MAKE) install -C i18n
endif

uninstall: uninstall-bin
	$(MAKE) uninstall -C share
ifdef I18N
	$(MAKE) uninstall -C i18n
endif

clean:
	rm -f *.o $(OUTFILE) search core
	$(MAKE) clean -C unittest
	$(MAKE) clean -C i18n

distclean: clean
	rm -f Makefile.settings config.H axget-*.tar axget-*.tar.gz axget-*.tar.bz2

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
	mv $(LIBOUTFILE) unittest/lib/
	$(MAKE) -C unittest

ut: unittest  # build and run unit testcase
	cd unittest; ./axget_test

install-bin:
	mkdir -p $(DESTDIR)$(BINDIR)/
	cp $(OUTFILE) $(DESTDIR)$(BINDIR)/$(OUTFILE)

uninstall-bin:
	rm -f $(BINDIR)/$(OUTFILE)

tar:
	version=`sed -n 's/#define AXEL_VERSION_STRING[ \t]*"\([^"]*\)"/\1/p' < axel.h` && \
	tar --create --numeric-owner --owner 0 --group 0 --transform "s#^#axget-$${version}/#" "--file=axget-$${version}.tar" --exclude-vcs -- src i18n share configure Makefile axelrc.example README.md && \
	gzip --best < "axget-$${version}.tar" > "axget-$${version}.tar.gz" && \
	bzip2 --best < "axget-$${version}.tar" > "axget-$${version}.tar.bz2"

ctags:
	ctags --languages=c --fields=+SKz --sort=foldcase *.c

