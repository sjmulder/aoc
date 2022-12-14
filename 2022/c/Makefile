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

day07: day07.o compat/stdio.o compat/string.o
	${LINK.c} -o day07 day07.o compat/stdio.o compat/string.o ${LDLIBS}
day12: day12.o compat/stdio.o
	${LINK.c} -o day12 day12.o compat/stdio.o ${LDLIBS}

day07.o: compat/stdio.h compat/string.h
day12.o: compat/stdio.h compat/util.h

compat/stdio.o: compat/stdio.c compat/stdio.h
	${COMPILE.c} -o compat/stdio.o compat/stdio.c
compat/string.o: compat/string.c compat/string.h
	${COMPILE.c} -o compat/string.o compat/string.c

.PHONY: all run clean
