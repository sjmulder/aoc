CFLAGS+=	-Wall -Werror
CFLAGS+=	-O2

ALL+=		solve1 solve1-sort
ALL+=		solve2 solve2-sort solve2-bsearch
ALL+=		gen-input input.gen

all: ${ALL}

clean:
	rm -f ${ALL}

input.gen: gen-input
	./gen-input >input.gen

.PHONY: all clean
