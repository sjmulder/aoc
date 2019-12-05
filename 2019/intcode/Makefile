CFLAGS += -Wall -Wextra

BIN        = interp
OBJ_interp = interp.o intcode.o

all: $(BIN)

clean:
	rm -f $(BIN) *.o

interp: $(OBJ_interp)
	$(LINK.c) -o interp $(OBJ_interp) $(LDLIBS)

interp.o:  intcode.h
intcode.o: intcode.h

.PHONY: all clean
