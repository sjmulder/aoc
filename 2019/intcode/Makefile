CFLAGS += -Wall -Wextra

BIN        = interp intas
OBJ_interp = interp.o intcode.o
OBJ_intas  = intas.o intcode.o

all: $(BIN)

clean:
	rm -f $(BIN) *.o

interp: $(OBJ_interp)
	$(LINK.c) -o interp $(OBJ_interp) $(LDLIBS)

intas: $(OBJ_intas)
	$(LINK.c) -o intas $(OBJ_intas) $(LDLIBS)

interp.o:  intcode.h
intas.o:   intcode.h
intcode.o: intcode.h

.PHONY: all clean
