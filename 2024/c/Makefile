# Use BSD make (bmake)

YEAR=		2024
DAYS=		day01 day02 day03 day04

PROGS=		${DAYS} \
		day02-orig

CFLAGS+=	-O3 -Wall -Wextra
#CFLAGS+=	-g -ftrapv -Wconversion

all: ${PROGS}

clean:
	rm -f ${PROGS} *.o

day01: day01.c common.h
day02: day02.c common.h
day03: day03.c common.h
day04: day04.c common.h

day02-orig: day02-orig.c common.h

.PHONY: all clean

.include "../../mk/common.mk"
