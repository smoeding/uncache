# Makefile.  Generated from Makefile.in by configure.

package = uncache
version = 1.1.0
tarname = uncache
distdir = $(tarname)-$(version)

prefix      = /usr/local
exec_prefix = ${prefix}
bindir      = ${exec_prefix}/bin

# Tool-specific substitution variables
CC		= gcc
CFLAGS		= -g -O2 -Wall -Wextra -Wformat
CPPFLAGS	= 
INSTALL		= /usr/bin/install -c
INSTALL_DATA	= ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT	= ${INSTALL}

all: uncache

clean:

realclean: clean
	$(RM) uncache configure.scan autoscan.log config.log

distclean: realclean
	$(RM) *~

install: uncache
	$(INSTALL_PROGRAM) -s -D uncache $(DESTDIR)/usr/bin/uncache
	$(INSTALL_DATA) -D uncache.8 $(DESTDIR)/usr/man/man8/uncache.8
	gzip -9 $(DESTDIR)/usr/man/man8/uncache.8

dist:
	tar czf uncache-$(version).tar.gz LICENSE Makefile uncache.c uncache.8

Makefile: Makefile.in config.status
	./config.status $@

config.status: configure
	./config.status --recheck

.PHONY: all clean realclean distclean install dist
