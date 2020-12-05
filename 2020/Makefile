DIRS!=	find . -name day\*
DAYS=	${DIRS:S,./,,}
CLEANS=	${DAYS:S,^,clean-,}

all:	${DAYS}
clean:	${CLEANS}

.for day in ${DAYS}
${day}:		; ${MAKE} -C ${day}
clean-${day}:	; ${MAKE} -C ${day} clean
.endfor

.PHONY: all ${DAYS} ${CLEANS}
