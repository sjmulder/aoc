# Use BSD make (bmake)

YEAR=		2023
DAYS=		day01 day02 day03 day04 day05 day06 day07 day08 day09 \
		day10 day11 day12 day13 day14 day15 day16 day17 day18 \
		day19 day20 day21 day22 day23 day24
VIS_SUPPORT=	day14
VIS_PROGS=	${VIS_SUPPORT:S/$/-vis/}

PROGS=		${DAYS} ${VIS_PROGS} \
		day01-orig day02-bychar day06-brute day10-flood \
		day18-p1 day21-multi day25-wish

VIS=		day14.mp4

.if "${WITH_DOT}" != "0"
VIS+=		day20.dot day20.pdf
.endif

CFLAGS+=	-O3 -Wall -Wextra
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

day01-orig:   day01-orig.c   common.h
day02-bychar: day02-bychar.c common.h
day06-brute:  day06-brute.c  common.h
day10-flood:  day10-flood.c  common.h
day18-p1:     day18-p1.c     common.h
day20-dot:    day20-dot.c    common.h
day25-wish:   day25-wish.c   common.h

.for p in ${VIS_SUPPORT}
$p-vis: $p.c vis.c vis.h
	${LINK.c} -DWITH_VIS=1 -o $p-vis $p.c vis.c ${LDLIBS}
.endfor

day14.mp4: day14-vis ; ./day14-vis <${INPUT.day14}
day20.dot: day20-dot ; ./day20-dot <${INPUT.day20} >$@
day20.pdf: day20.dot ; dot -T pdf day20.dot >$@

.PHONY: all clean vis

.include "../../mk/common.mk"
