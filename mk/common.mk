# Implements some common things including run and bench targetsL
#
#   run   [DAYS=..] [INPUT=..]
#   bench [DAYS=..] [INPUT=..] [BENCH=..]
#
# Both are run with input files derrived from the program names, this
# can be overridden with INPUT or INPUT.<name> (e.g. INPUT.day12).

UNAME!=		uname

.if "${UNAME}" == "Darwin"
TIME?=		time
SHELL=		zsh
.elif "${UNAME}" == "OpenBSD"
TIME?=		time
.endif

Q?=		@
TIME?=		/usr/bin/time -f ${TIMEFMT}
TIMEFMT?=	" %E  %M Kb  %F+%R faults"

ENV+=		TIMEFMT=${TIMEFMT}

.for p in ${DAYS}
. if "${INPUT}" != ""
INPUT.$p?=	${INPUT}
. else
INPUT.$p?=	../../inputs/${YEAR}/${p:S/day//:S/-p1//:S/.py//:S/.js//}-input.txt
. endif

RUN_CMD+=	./$p <${INPUT.$p};
BENCH_CMD+=	printf "$p "; ${TIME} ./$p <${INPUT.$p} >/dev/null;
.endfor

run:   ${DAYS} ; $Q env ${ENV} ${SHELL} -c ${RUN_CMD:Q}
bench: ${DAYS} ; $Q env ${ENV} ${SHELL} -c ${BENCH_CMD:Q}

show-var: ; @echo ${${VARNAME}}

.o:
	${LINK.c} -o $@ $> ${LDLIBS}

.PHONY: run bench show-var
