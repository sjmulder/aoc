PROGS+=	day17

CC=		clang
CFLAGS+=	-Wall -Wextra
CFLAGS+=	-O3
CFLAGS+=	-Wno-empty-body
CFLAGS+=	-Wno-implicit-function-declaration
CFLAGS+=	-Wno-implicit-int
CFLAGS+=	-Wno-misleading-indentation
CFLAGS+=	-Wno-parentheses

all: ${PROGS}

clean:
	rm -f ${PROGS}

run: all
	@./all.sh

.PHONY: all clean
