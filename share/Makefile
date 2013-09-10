################################
##      Makefile for Axget    ##
##                            ##
## Copyright 2001-2012 Lintux ##
## Copyright 2013      Freyr  ##
################################

### DEFINITIONS

-include ../Makefile.settings

install: install-man install-etc
uninstall: uninstall-man uninstall-etc

install-man:
	mkdir -p $(DESTDIR)$(MANDIR)/man1/
	cp man/axget.1 $(DESTDIR)$(MANDIR)/man1/axget.1
	mkdir -p $(DESTDIR)$(MANDIR)/zh_CN/man1/
	cp man/axget_zh_CN.1 $(DESTDIR)$(MANDIR)/zh_CN/man1/axget.1

uninstall-man:
	rm -f $(MANDIR)/man1/axget.1
	rm -f $(MANDIR)/zh_CN/man1/axget.1

install-etc:
	mkdir -p $(DESTDIR)$(ETCDIR)/
	cp etc/axelrc.example $(DESTDIR)$(ETCDIR)/axelrc

uninstall-etc:
	rm -f $(ETCDIR)/axelrc

