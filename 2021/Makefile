SUBDIRS=	c c-golf cs

.if "${WITH_BASIC}" == "1"
SUBDIRS+=	c64-basic
.endif

.if "${WITH_CPP17}" != "0"
SUBDIRS+=	cpp
.endif

.include "../mk/subdirs.mk"
