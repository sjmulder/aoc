# Use BSD make (bmake)

PROGS+=	day01 day02 day03 day04 day05 day06 day06-orig day07 \
	day07-orig day08 day08-orig day09 day10 day11 day12 day13 \
	day14 day15 day15-tests

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-O3

LDLIBS+=	-lm

all: ${PROGS}

run: all
	@./all.sh

clean:
	rm -f ${PROGS} *.o
	rm -rfv *.dSYM

day07: day07.o stdio.o string.o
day12: day12.o stdio.o

day07.o: day07.c compat/stdio.h compat/string.h
day12.o: day12.c compat/stdio.h compat/util.h
day14.o: day14.c compat/util.h
day15.o: day15.c compat/util.h

vis.o: vis.c vis.h
stdio.o: compat/stdio.c compat/stdio.h
string.o: compat/string.c compat/string.h

.if "${WITH_VIS}" == "1"
PROGS+=		day12-vis
CFLAGS+=	-DWITH_VIS=1

day14: vis.o
day14.o: vis.h
.endif

.o:
	${LINK.c} -o $@ $> ${LDLIBS}

.PHONY: all run clean
