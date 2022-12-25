SUBDIRS=	c c-golf awk

.if "${WITH_BASIC}" == "1"
SUBDIRS+=	c64-basic
.endif

.if "${WITH_MONO}" != "0"
SUBDIRS+=	cs
.endif

.include "../mk/subdirs.mk"
