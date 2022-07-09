PROGS+=	day01 day02 day03-p1 day03-p2 day04 day05-p1 day05-p2 \
	day06-p1 day06-p2 day08 day09 day10 day11 day12-p1 day12-p2 \
	day13 day14 day15 day16 day17 \
	day13-noname day14-p1 wiresim wiresim-declops 

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-O3

LDLIBS+=	-lm -lcrypto

all: ${PROGS}

run: all
	@./all.sh

clean:
	rm -f ${PROGS} *.o
	rm -rfv *.dSYM

.PHONY: all run clean
