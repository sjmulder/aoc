all: c c-golf cs

c:	; ${MAKE} -C c
c-golf:	; ${MAKE} -C c-golf
cs:	; ${MAKE} -C cs

clean:
	${MAKE} -C c clean
	${MAKE} -C c-golf clean
	${MAKE} -C cs clean

.PHONY: all clean c c-golf cs
