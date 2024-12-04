SUBDIRS=	c awk
TARGETS=	all clean run

.if "${WITH_MONO}" != "0"
SUBDIRS+=	cs
.endif

bench:	; @echo "'bench' not implemented for 2015" >&2

.include "../mk/subdirs.mk"
