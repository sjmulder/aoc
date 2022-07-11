PROGS+=	day01 day02 day03

CXXFLAGS+=	-Wall -Wextra
CXXFLAGS+=	-O3

all: ${PROGS}

run: all
	@./all.sh

clean:
	rm -f ${PROGS} *.o
	rm -rfv *.dSYM

.PHONY: all run clean
