# Makefile for uncache

VERSION	=1.0.1

CFLAGS	= -Wall -DUNCACHE_VERSION=\"$(VERSION)\"

all:	uncache

uncache: uncache.o

install:	uncache
	install -s -D uncache $(DESTDIR)/usr/bin/uncache
	install -D uncache.8 $(DESTDIR)/usr/man/man8/uncache.8
	gzip -9 $(DESTDIR)/usr/man/man8/uncache.8

clean:
	$(RM) uncache.o

realclean:	clean
	$(RM) uncache

dist:
	tar czf uncache-$(VERSION).tar.gz LICENSE Makefile uncache.c uncache.8

.PHONY: all install clean realclean
