# Use BSD make (bmake)

YEAR=		2024
DAYS=		day01 day02 day03 day04 day05 day06 day07 day08 day09 \
		day10 day11 day12 day13 day14 day15 day16 day17 day18
VIS_PROGS=	day10-vis day14-vis

PROGS=		${DAYS} ${VIS_PROGS} \
		day02-p1 day17-p1

VIS=		day10.mp4 day14.mp4

CFLAGS+=	-O3
CFLAGS+=	-Wall -Wextra -Wno-misleading-indentation
#CFLAGS+=	-g -ftrapv -Wconversion
LDLIBS+=	-lm

all: ${PROGS}

vis: ${VIS}

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
day11: day11.c common.h
day12: day12.c common.h
day13: day13.c common.h
day14: day14.c common.h
day15: day15.c common.h
day16: day16.c common.h
day17: day17.c common.h
day18: day18.c common.h

day02-p1: day02-p1.c common.h
day17-p1: day17-p1.c common.h

day10-vis: day10-vis.o vis.o
	${CC} ${LDFLAGS} -o day10-vis day10-vis.o vis.o ${LDLIBS}

day14-vis: day14-vis.o vis.o
	${CC} ${LDFLAGS} -o day14-vis day14-vis.o vis.o ${LDLIBS}

day10-vis.o: day10-vis.c common.h vis.h
day14-vis.o: day14-vis.c common.h vis.h

vis.o: vis.c vis.h

day10.mp4: day10-vis ; ./day10-vis <${INPUT.day10}
day14.mp4: day14-vis ; ./day14-vis <${INPUT.day14}

.PHONY: all clean vis

.include "../../mk/common.mk"
