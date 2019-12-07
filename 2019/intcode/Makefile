CFLAGS += -Wall -Wextra

ALL        = libintcode.so interp intas
OBJ_lib    = intcode.o
OBJ_interp = interp.o intcode.o
OBJ_intas  = intas.o intcode.o

all: $(ALL)

clean:
	rm -f $(ALL) *.o

libintcode.so: $(OBJ_lib)
	$(LINK.c) -shared -o libintcode.so $(OBJ_lib) $(LDLIBS)

interp: $(OBJ_interp)
	$(LINK.c) -o interp $(OBJ_interp) $(LDLIBS)

intas: $(OBJ_intas)
	$(LINK.c) -o intas $(OBJ_intas) $(LDLIBS)

interp.o:  intcode.h
intas.o:   intcode.h
intcode.o: intcode.h

.PHONY: all clean
