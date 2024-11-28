SUBDIRS=	c python

.if "${WITH_CPP17}" != "0"
SUBDIRS+=	cpp
.endif

.if "${WITH_DOTNET}" != "0"
SUBDIRS+=	cs
.endif

.if "${WITH_GOLF}" != "0"
SUBDIRS+=	c-golf
.endif

.include "../mk/subdirs.mk"
