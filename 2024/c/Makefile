# Use BSD make (bmake)

YEAR=	2024
DAYS=	day01 day02 day03 day04 day05 day06 day07 day08 day09 day10

PROGS=	${DAYS} \
	day02-p1

CFLAGS+=	-O3 -Wall -Wextra
CFLAGS+=	-Wno-misleading-indentation
#CFLAGS+=	-g -ftrapv -Wconversion

all: ${PROGS}

clean:
	rm -f ${PROGS} *.o

day01: day01.c common.h
day02: day02.c common.h
day03: day03.c common.h
day04: day04.c common.h
day05: day05.c common.h
day06: day06.c common.h
day07: day07.c common.h
day08: day08.c common.h
day09: day09.c common.h
day10: day10.c common.h

day02-orig: day02-orig.c common.h

.PHONY: all clean

.include "../../mk/common.mk"
