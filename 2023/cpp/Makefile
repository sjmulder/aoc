# Use BSD make (bmake)

YEAR=		2023
DAYS=		day14
PROGS=		${DAYS}

CXXFLAGS+=	-O3 -Wall -Wextra
#CXXFLAGS+=	-g -ftrapv -Wconversion
LDLIBS+=	-lm

all: ${PROGS}

clean:
	rm -f ${PROGS} *.o

.PHONY: all clean

.include "../../mk/common.mk"
