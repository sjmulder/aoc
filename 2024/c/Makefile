# Use BSD make (bmake)

YEAR=		2024
DAYS=		day01

PROGS=		${DAYS}

CFLAGS+=	-O3 -Wall -Wextra
#CFLAGS+=	-g -ftrapv -Wconversion

all: ${PROGS}

clean:
	rm -f ${PROGS} *.o

day01: day01.c common.h

.PHONY: all clean

.include "../../mk/common.mk"
