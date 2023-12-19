SUBDIRS=	c c-golf python

.if "${WITH_CPP17}" != "0"
SUBDIRS+=	cpp
.endif

.if "${WITH_DOTNET}" != "0"
SUBDIRS+=	cs
.endif

.include "../mk/subdirs.mk"
