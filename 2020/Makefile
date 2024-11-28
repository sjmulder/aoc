SUBDIRS=	c awk

.if "${WITH_BASIC}" == "1"
SUBDIRS+=	c64-basic
.endif

.if "${WITH_MONO}" != "0"
SUBDIRS+=	cs
.endif

.if "${WITH_GOLF}" != "0"
SUBDIRS+=	c-golf
.endif

.include "../mk/subdirs.mk"
