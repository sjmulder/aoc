PROGS+=	day02.exe day03.exe day05.exe day06.exe day10-p1.exe

CSC?=	mcs

all: ${PROGS}

clean:
	rm -f ${PROGS}

run: all
	@./all.sh

.PHONY: all clean run
.SUFFIXES: .cs .exe

.cs.exe:
	${CSC} -out:$@ $<
