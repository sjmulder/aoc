SUBDIRS=	c awk
TARGETS=	all clean run

.if "${WITH_BASIC}" == "1"
SUBDIRS+=	c64-basic
.endif

.if "${WITH_MONO}" != "0"
SUBDIRS+=	cs
.endif

.if "${WITH_GOLF}" != "0"
SUBDIRS+=	c-golf
.endif

bench:	; @echo "'bench' not implemented for 2020" >&2

.include "../mk/subdirs.mk"
