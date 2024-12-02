SUBDIRS?=
TARGETS?=	all clean run bench

.for subdir in ${SUBDIRS}
. for target in ${TARGETS}

${target}: ${target}-${subdir}

${target}-${subdir}:
	${MAKE} -C ${subdir} ${target}

.PHONY: ${target}-${subdir}

. endfor
.endfor

.PHONY: all clean run
