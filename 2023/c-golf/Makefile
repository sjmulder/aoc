YEAR=		2023
DAYS=		day01
PROGS=		${DAYS}

CC=		clang
CFLAGS+=	-Wall -Wextra -O3
CFLAGS+=	-Wno-empty-body
CFLAGS+=	-Wno-implicit-function-declaration
CFLAGS+=	-Wno-implicit-int
CFLAGS+=	-Wno-misleading-indentation
CFLAGS+=	-Wno-parentheses
CFLAGS+=	-Wno-unknown-warning-option

all: ${PROGS}

clean:
	rm -f ${PROGS} *.o

.PHONY: all clean

.include "../../mk/common.mk"
