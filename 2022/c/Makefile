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

day07: day07.o stdio.o string.o
day11: day11.o string.o
day12: day12.o stdio.o
day13: day13.o stdio.o
day21: day21.o string.o

day14-vis: day14-vis.o vis.o
day18-vis: day18-vis.o vis.o
day20-vis: day20-vis.o vis.o
day22-vis: day22-vis.o vis.o
day23-vis: day23-vis.o vis.o
day24-vis: day24-vis.o vis.o

day07.o: day07.c compat/stdio.h compat/string.h
day11.o: day11.c compat/stdint.h compat/string.h compat/inttypes.h \
         compat/util.h
day12.o: day12.c compat/util.h
day13.o: day13.c compat/stdio.h
day14.o: day14.c compat/util.h
day15.o: day15.c compat/util.h compat/stdint.h compat/inttypes.h
day17.o: day17.c compat/stdint.h compat/inttypes.h
day18.o: day18.c compat/stdint.h
day19.o: day19.c compat/util.h
day20.o: day20.c compat/stdint.h compat/inttypes.h
day21.o: day21.c compat/stdint.h compat/string.h compat/inttypes.h
day22.o: day22.c compat/stdint.h compat/util.h
day24.o: day24.c compat/stdint.h
day25.o: day25.c compat/stdint.h

vis.o:    vis.c vis.h
stdio.o:  compat/stdio.c compat/stdio.h
string.o: compat/string.c compat/string.h

day14-vis.o: day14.c vis.h ; ${COMPILE.c} -o $@ -DWITH_VIS=1 day14.c
day18-vis.o: day18.c vis.h ; ${COMPILE.c} -o $@ -DWITH_VIS=1 day18.c
day20-vis.o: day20.c vis.h ; ${COMPILE.c} -o $@ -DWITH_VIS=1 day20.c
day22-vis.o: day22.c vis.h ; ${COMPILE.c} -o $@ -DWITH_VIS=1 day22.c
day23-vis.o: day23.c vis.h ; ${COMPILE.c} -o $@ -DWITH_VIS=1 day23.c
day24-vis.o: day24.c vis.h ; ${COMPILE.c} -o $@ -DWITH_VIS=1 day24.c

day14-vis.o: compat/util.h
day18-vis.o: compat/stdint.h
day20-vis.o: compat/stdint.h compat/inttypes.h
day22-vis.o: compat/stdint.h compat/util.h
day24-vis.o: compat/stdint.h

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
