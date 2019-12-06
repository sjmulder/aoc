CFLAGS += -Wall -Wextra

all: solve input.big

clean:
	rm -f solve mkbig input.big

input.big: mkbig
	./mkbig >input.big

.PHONY: all clean
