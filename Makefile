FILES!=	find . -name Makefile \
	 | grep '^./20../day../Makefile$$' \
	 | grep -v 2019/day07/ \
	 | grep -v 2019/day10/
DAYS=	${FILES:S,./,,:S,/Makefile,,}
CLEANS=	${DAYS:S,^,clean-,}

all:	${DAYS}
clean:	${CLEANS}

.for day in ${DAYS}
${day}:		; ${MAKE} -C ${day}
clean-${day}:	; ${MAKE} -C ${day} clean
.endfor

.PHONY: all ${DAYS} ${CLEANS}
