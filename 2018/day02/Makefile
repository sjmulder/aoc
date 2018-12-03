CFLAGS += -O2
CFLAGS += -Wall -Wextra

all: day02a day02b

clean:
	rm -f day02a day02b

check: day02a day02b
	test 12 = `./day02a <sample-a`
	test fgij = `./day02b <sample-b`

solve: day02a day02b
	./day02a <input
	./day02b <input

.PHONY: all clean check solve
