PROGS+=	day09.exe

CS?=	mcs

all: ${PROGS}

run: all
	@./all.sh

clean:
	rm -f ${PROGS}


.PHONY:    all run clean
.SUFFIXES: .cs .exe

.cs.exe:
	${CS} ${CSFLAGS} -out:$@ $<
