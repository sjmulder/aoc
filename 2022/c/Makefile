PROGS+=	day01 day02 day03 day04 day05 day06 day06-orig day07 \
	day07-orig day08 day08-orig day09 day10 day11

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-O3

LDLIBS+=	-lm

all: ${PROGS}

run: all
	@./all.sh

clean:
	rm -f ${PROGS} *.o
	rm -rfv *.dSYM

.PHONY: all run clean
