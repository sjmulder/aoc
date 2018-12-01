CFLAGS += -Wall -Wextra

all: day01a day01b

clean:
	rm -f day01a day02b

check: day01a day01b
	test  3 = `echo '+1 -2 +3 +1' | ./day01a`
	test  3 = `echo '+1 +1 +1'    | ./day01a`
	test  0 = `echo '+1 +1 -2'    | ./day01a`
	test -6 = `echo '-1 -2 -3'    | ./day01a`
	test  0 = `cycle sample-b1 | ./day01b`
	test 10 = `cycle sample-b2 | ./day01b`
	test  5 = `cycle sample-b3 | ./day01b`
	test 14 = `cycle sample-b4 | ./day01b`

solve: day01a day01b
	./day01a <input
	cycle input | ./day01b

.PHONY: all clean check solve
