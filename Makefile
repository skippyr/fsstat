include config.mk

.PHONY: all clean install uninstall

all: fsstat

clean:
	rm -f fsstat;

install: all
	mkdir -p ${MAN1PATH} ${BINPATH};
	sed "s/\$${VERSION}/${VERSION}/" fsstat.1 > ${MAN1PATH}/fsstat.1
	cp fsstat ${BINPATH};

uninstall:
	rm -f ${MAN1PATH}/fsstat.1 ${BINPATH}/fsstat;

fsstat: fsstat.c
	${CC} ${CFLAGS} -o${@} ${^};
