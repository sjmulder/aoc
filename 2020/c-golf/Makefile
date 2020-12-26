PROGS+=	day01-p1 day10 day15

CC=		clang
CFLAGS+=	-Wall -Wextra
CFLAGS+=	-O3
CFLAGS+=	-Wno-empty-body
CFLAGS+=	-Wno-implicit-function-declaration
CFLAGS+=	-Wno-implicit-int

all: ${PROGS}

clean:
	rm -r ${PROGS}

.PHONY:  all clean
