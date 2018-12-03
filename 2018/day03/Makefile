CFLAGS += -Wall -Wextra

all: day03a day03b

clean:
	rm -f day03a

check: day03a day03b
	test 4 = `./day03a <sample`
	test 3 = `./day03b <sample`

solve: day03a day03b
	./day03a <input
	./day03b <input

.PHONY: all clean check solve
