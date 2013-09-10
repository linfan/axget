################################
##      Makefile for Axget    ##
##                            ##
## Copyright 2001-2012 Lintux ##
## Copyright 2013      Freyr  ##
################################

### DEFINITIONS

-include Makefile.settings

all: $(OUTFILE)
install: install-bin install-etc install-man
uninstall: uninstall-bin uninstall-etc uninstall-man

clean:
	rm -f *.o $(OUTFILE) search core
	$(MAKE) clean -C unittest
	$(MAKE) clean -C i18n

ifdef I18N
all:
	$(MAKE) all -C i18n
install:
	$(MAKE) install -C i18n
uninstall:
	$(MAKE) uninstall -C i18n
endif

distclean: clean
	rm -f Makefile.settings config.H axget-*.tar axget-*.tar.gz axget-*.tar.bz2

install-man:
	mkdir -p $(DESTDIR)$(MANDIR)/man1/
	cp axel.1 $(DESTDIR)$(MANDIR)/man1/axel.1
	mkdir -p $(DESTDIR)$(MANDIR)/zh_CN/man1/
	cp axel_zh_CN.1 $(DESTDIR)$(MANDIR)/zh_CN/man1/axel.1

uninstall-man:
	rm -f $(MANDIR)/man1/axel.1
	rm -f $(MANDIR)/zh_CN/man1/axel.1

install-etc:
	mkdir -p $(DESTDIR)$(ETCDIR)/
	cp axelrc.example $(DESTDIR)$(ETCDIR)/axelrc

uninstall-etc:
	rm -f $(ETCDIR)/axelrc

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

# build and run unit testcase
ut: unittest
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

