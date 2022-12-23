# Implements the following targets:
#
#   run   [DAYS=..] [INPUT=..]
#   bench [DAYS=..] [INPUT=..] [BENCH=..]
#
# Both are run with input files derrived from the program names, this
# can be overridden with INPUT or INPUT.<name> (e.g. INPUT.day12).

Q?=		@
BENCH?=		time -f "$p  %E  %M Kb  %F+%R faults"

run:   ${DAYS} ; $Q${RUN_CMD}
bench: ${DAYS} ; $Q${BENCH_CMD}

.for p in ${DAYS}
INPUT.$p?=	${INPUT:U../../inputs/2022/${p:S/day//}-input.txt}
RUN_CMD+=	./$p <${INPUT.$p};
BENCH_CMD+=	${BENCH} ./$p <${INPUT.$p} >/dev/null;
.endfor

.o:
	${LINK.c} -o $@ $> ${LDLIBS}
