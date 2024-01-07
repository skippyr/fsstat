VERSION:=v1.0.0
PKG:=mistureba
CC:=cc
CFLAGS:=-std=c99 -pedantic -Os -Wall
BINPATH:=/usr/local/bin
MAN1PATH:=/usr/local/man/man1

.PHONY: all clean install uninstall

all: fsstat

clean:
	rm -f fsstat;

install: all
	mkdir -p ${MAN1PATH} ${BINPATH};
	sed "s/\$${VERSION}/${VERSION}/; s/\$${PKG}/${PKG}/" fsstat.1 >\
	    ${MAN1PATH}/fsstat.1
	cp fsstat ${BINPATH};

uninstall:
	rm -f ${MAN1PATH}/fsstat.1 ${BINPATH}/fsstat;

fsstat: fsstat.c
	${CC} ${CFLAGS} -o${@} ${^};
