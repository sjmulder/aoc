# Use BSD make (bmake)

DAYS=	day01 day02 day03 day04 day05 day06 day07 day08 day09 day10 \
	day11 day12 day13 day14 day15 day16 day17 day18 day19 day20 \
	day21 day22 day23 day24 day25

PROGS=	${DAYS} \
	day06-orig day07-orig day08-orig day12-vis day14-vis \
	day15-tests day18-vis day18-recur day19-wip day20-vis \
	day21-dot day22-vis day23-vis day24-vis

VIS=	day14.mp4 day18.mp4 day20.mp4 day22-p1.mp4 day22-p2.mp4 \
	day23.mp4 day24.mp4
.if "${NO_DOT}" != "1"
VIS+=	day21.dot day21.pdf
.endif

CFLAGS+=	-O3 -Wall -Wextra
#CFLAGS+=	-g -ftrapv -Wconversion
LDLIBS+=	-lm

all: ${PROGS}
vis: ${VIS}

clean:
	rm -f ${PROGS} ${VIS} *.o
	rm -rfv *.dSYM

${PROGS}: common.h

day14-vis: day14.c vis.o vis.h ; ${LINK.c} -o $@ -DWITH_VIS=1 day14.c vis.o
day18-vis: day18.c vis.o vis.h ; ${LINK.c} -o $@ -DWITH_VIS=1 day18.c vis.o
day20-vis: day20.c vis.o vis.h ; ${LINK.c} -o $@ -DWITH_VIS=1 day20.c vis.o
day22-vis: day22.c vis.o vis.h ; ${LINK.c} -o $@ -DWITH_VIS=1 day22.c vis.o
day23-vis: day23.c vis.o vis.h ; ${LINK.c} -o $@ -DWITH_VIS=1 day23.c vis.o
day24-vis: day24.c vis.o vis.h ; ${LINK.c} -o $@ -DWITH_VIS=1 day24.c vis.o

day14.mp4:    day14-vis ; ./day14-vis <../../inputs/2022/14-input.txt
day18.mp4:    day18-vis ; ./day18-vis <../../inputs/2022/18-input.txt
day20.mp4:    day20-vis ; ./day20-vis <../../inputs/2022/20-input.txt
day21.dot:    day21-dot ; ./day21-dot <../../inputs/2022/21-input.txt >$@
day21.pdf:    day21.dot ; dot -T pdf $< >$@
day22-p1.mp4: day24-vis ; ./day22-vis <../../inputs/2022/22-input.txt
day22-p2.mp4: day24-vis ; ./day22-vis <../../inputs/2022/22-input.txt
day23.mp4:    day23-vis ; ./day23-vis <../../inputs/2022/23-input.txt
day24.mp4:    day24-vis ; ./day24-vis <../../inputs/2022/24-input.txt

.PHONY: all clean vis

.include "../../mk/common.mk"
