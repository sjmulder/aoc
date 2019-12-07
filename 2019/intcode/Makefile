DESTDIR ?=
PREFIX  ?= /usr/local

CFLAGS += -Wall -Wextra

ALL        = libintcode.so interp intas
OBJ_lib    = intcode.o
OBJ_interp = interp.o intcode.o
OBJ_intas  = intas.o intcode.o

all: $(ALL)

clean:
	rm -f $(ALL) *.o

install: all
	install -d $(DESTDIR)$(PREFIX)/bin \
	           $(DESTDIR)$(PREFIX)/lib \
	           $(DESTDIR)$(PREFIX)/include
	install -m755 interp intas  $(DESTDIR)$(PREFIX)/bin/
	install -m755 libintcode.so $(DESTDIR)$(PREFIX)/lib/
	install -m644 intcode.h     $(DESTDIR)$(PREFIX)/include/

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/interp \
	      $(DESTDIR)$(PREFIX)/bin/intas \
	      $(DESTDIR)$(PREFIX)/lib/libintcode.so \
	      $(DESTDIR)$(PREFIX)/include/intcode.h

libintcode.so: $(OBJ_lib)
	$(LINK.c) -shared -o libintcode.so $(OBJ_lib) $(LDLIBS)

interp: $(OBJ_interp)
	$(LINK.c) -o interp $(OBJ_interp) $(LDLIBS)

intas: $(OBJ_intas)
	$(LINK.c) -o intas $(OBJ_intas) $(LDLIBS)

interp.o:  intcode.h
intas.o:   intcode.h
intcode.o: intcode.h

.PHONY: all clean install uninstall
