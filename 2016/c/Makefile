# day01-p2 is broken
PROGS=	day01-p1 day02 day03

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-O3

all: ${PROGS}

run: all
	@./all.sh

clean:
	rm -rf ${PROGS} *.o

.PHONY: all run clean
