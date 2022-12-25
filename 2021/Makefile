SUBDIRS=	c c-golf

.if "${WITH_BASIC}" == "1"
SUBDIRS+=	c64-basic
.endif

.if "${WITH_CPP17}" != "0"
SUBDIRS+=	cpp
.endif

.if "${WITH_DOTNET}" != "0"
SUBDIRS+=	cs
.endif

.include "../mk/subdirs.mk"
