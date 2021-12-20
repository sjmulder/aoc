PROGS+=	day01 day02 day03 day04 day05 day06 day07 day08 day09 day10 \
	day11 day12 day13 day14 day15 day16 day17 day18 day19 day20 \
	day06-orig day07-orig day08-p1 day11-short day11-mp4 \
	day15-astar day16-orig day17-fork

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-O3

LDLIBS+=	-lm

all: ${PROGS}

run: all
	@./all.sh

clean:
	rm -rf ${PROGS} *.o *.dSYM

.PHONY: all run clean
