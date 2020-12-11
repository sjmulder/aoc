CFLAGS+=	-Wall -Wextra
CFLAGS+=	-O2

GOLF_CC?=	clang
GOLF_CFLAGS+=	-Wno-empty-body
GOLF_CFLAGS+=	-Wno-implicit-function-declaration
GOLF_CFLAGS+=	-Wno-implicit-int

CSC?=		mcs
CSFLAGS+=

all: ${PROGS}

clean:
	rm -rf ${PROGS} *.dSYM

.PHONY:    all clean
.SUFFIXES: .cs .exe

.cs.exe:
	$(CSC) $(CSFLAGS) -out:$@ $<

# oh well
solve-golf: solve-golf.c
	$(GOLF_CC) $(GOLF_CFLAGS) -o solve-golf solve-golf.c
solve-golf-v2: solve-golf-v2.c
	$(GOLF_CC) $(GOLF_CFLAGS) -o solve-golf-v2 solve-golf-v2.c
solve1-golf: solve1-golf.c
	$(GOLF_CC) $(GOLF_CFLAGS) -o solve1-golf solve1-golf.c
solve2-golf: solve2-golf.c
	$(GOLF_CC) $(GOLF_CFLAGS) -o solve2-golf solve2-golf.c
