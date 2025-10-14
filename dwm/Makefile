#!/usr/bin/make -f

include ../config.mk

CFLAGS += -Wno-unused-parameter     # Default args on various utility functions
CFLAGS += -Wno-stringop-truncation  # Some interesting string logic...
CFLAGS += -D_XOPEN_SOURCE=700L
CFLAGS += -DVERSION=\"6.6\" -DXINERAMA
CFLAGS += -I/usr/X11R6/include -I/usr/include/freetype2

LDFLAGS += -L/usr/X11R6/lib -lX11 -lXinerama -lfontconfig -lXft

build: dwm

dwm: drw.o dwm.o util.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f dwm dwm.o drw.o util.o
