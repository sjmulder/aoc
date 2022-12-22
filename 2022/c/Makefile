# Use BSD make (bmake)

PROGS+=	day01 day02 day03 day04 day05 day06 day06-orig day07 \
	day07-orig day08 day08-orig day09 day10 day11 day12 day13 \
	day14 day15 day15-tests day16 day17 day18 day18-recur day19 \
	day19-wip day20 day21 day21-dot

RUN+=	day01 day02 day03 day04 day05 day06 day07 day08 day09 day10 \
	day11 day12 day13 day14 day15 day16 day17 day18 day19 day20 \
	day21

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-O3
#CLFAGS+=	-Wconversion
#CFLAGS+=	-g -ftrapv

LDLIBS+=	-lm

all: ${PROGS}

run: all
.for prog in ${RUN}
	@./${prog} <../../inputs/2022/${prog:S/day//}-input.txt
.endfor

bench:
.for prog in ${RUN}
	@time -f "${prog}  %E  %M Kb  %F+%R faults" ./${prog} \
	  <../../inputs/2022/${prog:S/day//}-input.txt \
	  >/dev/null
.endfor

clean:
	rm -f ${PROGS} *.o day21.dot day21.pdf
	rm -rfv *.dSYM

day07: day07.o stdio.o string.o
day12: day12.o stdio.o

day07.o: day07.c compat/stdio.h compat/string.h
day12.o: day12.c compat/stdio.h compat/util.h
day14.o: day14.c compat/util.h
day15.o: day15.c compat/util.h

day21.dot: day21-dot ../data/21-input.txt
	./day21-dot <../data/21-input.txt >$@

vis.o: vis.c vis.h
stdio.o: compat/stdio.c compat/stdio.h
string.o: compat/string.c compat/string.h

.if "${WITH_VIS}" == "1"
PROGS+=		day12-vis
CFLAGS+=	-DWITH_VIS=1

day14: vis.o
day18: vis.o

day14.o: vis.h
day18.o: vis.h
.endif

.SUFFIXES: .dot .pdf

.dot.pdf:
	dot -T pdf $< >$@

.o:
	${LINK.c} -o $@ $> ${LDLIBS}

.PHONY: all run bench clean
