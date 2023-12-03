# Use BSD make (bmake)

YEAR=		2023
DAYS=		day01 day02 day03
PROGS=		${DAYS} day01-orig day02-bychar

CFLAGS+=	-O3 -Wall -Wextra
#CFLAGS+=	-g -ftrapv -Wconversion
LDLIBS+=	-lm

all: ${PROGS}

clean:
	rm -f ${PROGS} *.o

day01: day01.c common.h
day02: day02.c common.h
day03: day03.c common.h
day04: day04.c common.h
day05: day05.c common.h
day07: day07.c common.h
day08: day08.c common.h
day09: day09.c common.h
day10: day10.c common.h
day12: day12.c common.h
day13: day13.c common.h
day14: day14.c common.h
day15: day15.c common.h
day16: day16.c common.h
day17: day17.c common.h
day18: day18.c common.h
day19: day19.c common.h
day20: day20.c common.h
day21: day21.c common.h
day22: day22.c common.h
day23: day23.c common.h
day24: day24.c common.h
day25: day25.c common.h

day01-orig:   day01-orig.c   common.h
day02-bychar: day02-bychar.c common.h

.PHONY: all clean

.include "../../mk/common.mk"
