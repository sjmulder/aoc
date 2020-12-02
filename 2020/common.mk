CFLAGS+=	-Wall -Wextra
CFLAGS+=	-O2

CSC?=		mcs
CSFLAGS+=

all: ${PROGS}

clean:
	rm -f ${PROGS}

.PHONY:    all clean
.SUFFIXES: .cs .exe

.cs.exe:
	$(CSC) $(CSFLAGS) -out:$@ $<

