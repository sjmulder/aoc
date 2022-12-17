# Use BSD make (bmake)

PROGS=	day01-p1 day01-p2 day02-p1 day02-p2 day03-p1 day03-p2 day04-p1

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-O3

all: ${PROGS}

run: all
	@./all.sh

clean:
	rm -f ${PROGS} *.o

.PHONY: all run clean
