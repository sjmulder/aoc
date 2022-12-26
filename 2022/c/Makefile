# Use BSD make (bmake)

DAYS=	day01 day02 day03 day04 day05 day06 day07 day08 day09 day10 \
	day11 day12 day13 day14 day15 day16 day17 day18 day19 day20 \
	day21 day22 day23 day24 day25

VIS_SUPPORT=	day14 day18 day20 day22 day23 day24
VIS_PROGS=	${VIS_SUPPORT:S/$/-vis/}

PROGS=	${DAYS} ${VIS_PROGS} \
	day12-vis day06-orig day07-orig day08-orig \
	day15-tests day18-recur day19-wip day21-dot

VIS=	day14.mp4 day18.mp4 day20.mp4 day21.dot day21.pdf day22-p1.mp4 \
	day22-p2.mp4 day23.mp4 day24.mp4

CFLAGS+=	-O3 -Wall -Wextra
#CFLAGS+=	-g -ftrapv -Wconversion
LDLIBS+=	-lm

all: ${PROGS}

vis: ${VIS}

clean:
	rm -f ${PROGS} ${VIS} *.o
	rm -rfv *.dSYM

${PROGS}: common.h

.for p in ${VIS_SUPPORT}
$p-vis: $p.c vis.c vis.h
	${LINK.c} -DWITH_VIS=1 -o $p-vis $p.c vis.c ${LDLINK}
.endfor

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
