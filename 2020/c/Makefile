PROGS+=	day01 day02 day03 day04 day05 day06 day07 day08 day09 day10 \
	day11 day12 day13 day14 day15 day16 day17 day18 day19 day20 \
	day21 day22 day23 day24 day25

PROGS+= day13-p2-brute

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-Wno-unused-result
CFLAGS+=	-O3
CFLAGS+=	-MMD

all: ${PROGS}

clean:
	rm -f ${PROGS} *.o *.d

run: all
	@./all.sh

.c:
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $< ${LDFLAGS}
day14: day14.o day14-p1.o day14-p2.o
	${CC} ${LDFLAGS} -o $@ day14.o day14-p1.o day14-p2.o ${LDFLAGS}
day17: day17.o day17-p1.o day17-p2.o
	${CC} ${LDFLAGS} -o $@ day17.o day17-p1.o day17-p2.o ${LDFLAGS}

-include *.d

.PHONY: all clean run
