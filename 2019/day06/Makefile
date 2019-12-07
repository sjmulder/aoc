CFLAGS += -Wall -Wextra

all: solve solve-v2 input.big

clean:
	rm -f solve solve-v2 mkbig input.big

input.big: mkbig
	./mkbig >input.big

.PHONY: all clean
