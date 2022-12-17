# Use BSD make (bmake)

PROGS=	day01-p1 day01-p2 day02-p1 day02-p2 day03-p1 day03-p2 day04-p1 \
	day04-p2 day08 day09 day10-p1 day10-p2 day11-p1 day11-p2 day12-p1 \
	day12-p2 day13-p1 day13-p2 day14-p1 day14-p2 day15-p1 day15-p2 \
	day20-p1

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-O3

all: ${PROGS}

run: all
	@./all.sh

clean:
	rm -f ${PROGS} *.o

day14-p1: day14-p1.o khash.o
day14-p2: day14-p2.o khash.o

day14-p1.o: day14-p1.c khash.h
day14-p2.o: day14-p1.c khash.h
khash.o: khash.h

.o:
	${LINK.c} -o $@ $> ${LDLIBS}

.PHONY: all run clean
