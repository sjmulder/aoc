# Use BSD make (bmake)

PROGS+=	day01 day02 day03 day04 day05 day06 day06-orig day07 \
	day07-orig day08 day08-orig day09 day10 day11 day12 day12-vis \
	day13

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

stdio.o: compat/stdio.c compat/stdio.h
string.o: compat/string.c compat/string.h

.o:
	${LINK.c} -o $@ $> ${LDLIBS}

.PHONY: all run clean
