# day10 not yet ready
SUBDIRS=	day01 day02 day03 day04 day06 day07 day08 intcode
TARGETS=	all clean

.if "${WITH_DOTNET}" != "0"
SUBDIRS+=	day08-cs day08-fs
.endif

all:

all-day07: all-intcode

run:
	echo "'run' not implemented for 2019" >&2

.include "../mk/subdirs.mk"
