CFLAGS += -Wall -Wextra -ansi -O2

all: day14a day14b
clean: ; rm -f day14a day14b *.o

day14a: day14a.o khash.o
	$(CC) $(LDFLAGS) -o day14a day14a.o khash.o $(LDLIBS)

day14b: day14b.o khash.o
	$(CC) $(LDFLAGS) -o day14b day14b.o khash.o $(LDLIBS)

khash.o: khash.h
day14a.o: khash.h
day14b.o: khash.h

.PHONY: all clean
