# Use BSD make (bmake)

DAYS=	day01 day02 day03 day04 day05 day06 day07 day08 day09 day10 \
	day11 day12 day13 day14 day15 day16 day17 day18 day19 day20 \
	day21

PROGS=	${DAYS} \
	day06-orig day07-orig day08-orig day12-vis day14-vis \
	day15-tests day18-vis day18-recur day19-wip day21-dot

CFLAGS+=	-O3 -Wall -Wextra
#CFLAGS+=	-g -ftrapv -Wconversion
LDLIBS+=	-lm

all: ${PROGS}

clean:
	rm -f ${PROGS} *.o day21.dot day21.pdf
	rm -rfv *.dSYM

day07: day07.o stdio.o string.o
day12: day12.o stdio.o

day14-vis: day14-vis.o vis.o
day18-vis: day18-vis.o vis.o

day07.o: day07.c compat/stdio.h compat/string.h
day12.o: day12.c compat/stdio.h compat/util.h
day14.o: day14.c compat/util.h
day15.o: day15.c compat/util.h

vis.o:    vis.c vis.h
stdio.o:  compat/stdio.c compat/stdio.h
string.o: compat/string.c compat/string.h

day14-vis.o: day14.c vis.h
	${COMPILE.c} -o $@ -DWITH_VIS=1 day14.c
day18-vis.o: day18.c vis.h
	${COMPILE.c} -o $@ -DWITH_VIS=1 day18.c

day21.dot: day21-dot ../data/21-input.txt
	./day21-dot <../data/21-input.txt >$@
day21.pdf: day12.dot
	dot -T pdf $< >$@

.PHONY: all clean

.include "../../mk/common.mk"
