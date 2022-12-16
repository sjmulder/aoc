SUBDIRS?=	2015 2020 2021 2022
TARGETS?=	all clean run

.for subdir in ${SUBDIRS}
. for target in ${TARGETS}

${target}: ${target}-${subdir}

${target}-${subdir}:
	${MAKE} -C ${subdir} ${target}

.PHONY: ${target}-${subdir}

. endfor
.endfor

.PHONY: all clean run
