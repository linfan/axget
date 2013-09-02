###########################
##   Makefile for Axel   ##
##                       ##
## Copyright 2001 Lintux ##
###########################

### DEFINITIONS

-include Makefile.settings

.SUFFIXES: .po .mo

# Add your translation here..
MOFILES = nl.mo de.mo ru.mo zh_CN.mo


all: $(OUTFILE)
install: install-bin install-etc install-man
uninstall: uninstall-bin uninstall-etc uninstall-man

clean:
	rm -f *.mo *.o $(OUTFILE) search core
	$(MAKE) clean -C unittest

distclean: clean
	rm -f Makefile.settings config.H axel-*.tar axel-*.tar.gz axel-*.tar.bz2

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
	tar --create --numeric-owner --owner 0 --group 0 --transform "s#^#axel-$${version}/#" "--file=axel-$${version}.tar" --exclude-vcs -- *.c *.h *.po *.1 configure Makefile axelrc.example README.md && \
	gzip --best < "axel-$${version}.tar" > "axel-$${version}.tar.gz" && \
	bzip2 --best < "axel-$${version}.tar" > "axel-$${version}.tar.bz2"

ctags:
	ctags --languages=c --fields=+SKz --sort=foldcase *.c

### I18N FILES

%.po:
	-@mv $@ $@.bak
	xgettext -k_ -o$@ *.[ch]
	@if [ -e $@.bak ]; then \
		echo -n Merging files...; \
		msgmerge -vo $@.combo $@.bak $@; \
		rm -f $@ $@.bak; \
		mv $@.combo $@; \
	fi

.po.mo: $@.po
	msgfmt -vo $@ $*.po

i18n-mofiles: $(MOFILES)

install-i18n:
	@echo Installing locale files...
	@for i in $(MOFILES); do \
		mkdir -p $(DESTDIR)$(LOCALE)/`echo $$i | cut -d. -f1`/LC_MESSAGES/; \
		cp $$i $(DESTDIR)$(LOCALE)/`echo $$i | cut -d. -f1`/LC_MESSAGES/axel.mo; \
	done

uninstall-i18n:
	cd $(LOCALE); find . -name axel.mo -exec 'rm' '{}' ';'
