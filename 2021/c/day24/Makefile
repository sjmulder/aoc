AWK?=	awk

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-O3

all: day24

clean:
	rm -f day24 input.c *.o

day24: day24.o input.o
	${CC} ${LDFLAGS} -o day24 day24.o input.o ${LDLIBS}

input.c: input.txt compile.awk
	${AWK} -f compile.awk input.txt >input.c

.PHONY: all clean
