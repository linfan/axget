################################
##      Makefile for Axget    ##
##                            ##
## Copyright 2001-2012 Lintux ##
## Copyright 2013      Freyr  ##
################################

### DEFINITIONS

.SUFFIXES: .po .mo

# Add your translation here..
MOFILES = nl.mo de.mo ru.mo zh_CN.mo

all: i18n-mofiles
install: install-i18n
uninstall: uninstall-i18n

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

clean:
	rm -f *.mo 

install-i18n:
	@echo Installing locale files...
	@for i in $(MOFILES); do \
		mkdir -p $(DESTDIR)$(LOCALE)/`echo $$i | cut -d. -f1`/LC_MESSAGES/; \
		cp $$i $(DESTDIR)$(LOCALE)/`echo $$i | cut -d. -f1`/LC_MESSAGES/axel.mo; \
	done

uninstall-i18n:
	cd $(LOCALE); find . -name axel.mo -exec 'rm' '{}' ';'

