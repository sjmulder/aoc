# Use BSD make (bmake)

YEAR=		2023
DAYS=		day01
PROGS=		${DAYS} day01-orig

CFLAGS+=	-O3 -Wall -Wextra
#CFLAGS+=	-g -ftrapv -Wconversion
LDLIBS+=	-lm

all: ${PROGS}

clean:
	rm -f ${PROGS} *.o

${PROGS}: common.h

.PHONY: all clean

.include "../../mk/common.mk"
