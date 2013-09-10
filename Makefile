################################
##      Makefile for Axget    ##
##                            ##
## Copyright 2001-2012 Lintux ##
## Copyright 2013      Freyr  ##
################################

### DEFINITIONS

-include Makefile.settings

all:
	$(MAKE) all -C src
ifdef I18N
	$(MAKE) all -C i18n
endif

install: install-bin
	$(MAKE) install -C src
	$(MAKE) install -C share
ifdef I18N
	$(MAKE) install -C i18n
endif

uninstall: uninstall-bin
	$(MAKE) uninstall -C src
	$(MAKE) uninstall -C share
ifdef I18N
	$(MAKE) uninstall -C i18n
endif

clean:
	$(MAKE) clean -C src
	$(MAKE) clean -C i18n
	$(MAKE) clean -C unittest

distclean: clean
	rm -f Makefile.settings src/config.H axget-*.tar axget-*.tar.gz axget-*.tar.bz2

ut:
	$(MAKE) unittest -C src
	$(MAKE) -C unittest

tar:
	version=`sed -n 's/#define AXEL_VERSION_STRING[ \t]*"\([^"]*\)"/\1/p' < axel.h` && \
	tar --create --numeric-owner --owner 0 --group 0 --file=axget-$${version}.tar -- src i18n share configure Makefile README.md && \
	gzip --best < "axget-$${version}.tar" > "axget-$${version}.tar.gz" && \
	bzip2 --best < "axget-$${version}.tar" > "axget-$${version}.tar.bz2"

tags:
	$(MAKE) tags -C src

