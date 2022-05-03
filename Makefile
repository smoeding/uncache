# Makefile for uncache

VERSION	 := 1.0.2

# Compile flags
CPPFLAGS := -DUNCACHE_VERSION=\"$(VERSION)\"
CFLAGS   := -Wall

.PHONY: all
all: uncache

.PHONY: clean
clean:

.PHONY: realclean
realclean: clean
	$(RM) uncache

.PHONY: install
install: uncache
	install -s -D uncache $(DESTDIR)/usr/bin/uncache
	install -D uncache.8 $(DESTDIR)/usr/man/man8/uncache.8
	gzip -9 $(DESTDIR)/usr/man/man8/uncache.8

.PHONY: dist
dist:
	tar czf uncache-$(VERSION).tar.gz LICENSE Makefile uncache.c uncache.8
