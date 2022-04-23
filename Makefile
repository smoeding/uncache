# Makefile for uncache

VERSION	 := 1.0.1

# Compile flags
CPPFLAGS := -DUNCACHE_VERSION=\"$(VERSION)\"
CFLAGS   := -Wall

all: uncache

install: uncache
	install -s -D uncache $(DESTDIR)/usr/bin/uncache
	install -D uncache.8 $(DESTDIR)/usr/man/man8/uncache.8
	gzip -9 $(DESTDIR)/usr/man/man8/uncache.8

clean:
# Nothing to do

realclean: clean
	$(RM) uncache

dist:
	tar czf uncache-$(VERSION).tar.gz LICENSE Makefile uncache.c uncache.8

.PHONY: all install clean realclean
