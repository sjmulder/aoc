all:
	${MAKE} -C c
	${MAKE} -C cpp
	${MAKE} -C c-golf
	${MAKE} -C cs
.if "${WITH_BASIC}" == "1"
	${MAKE} -C c64-basic
.endif

run:
	${MAKE} -C c run
	${MAKE} -C cpp run
	${MAKE} -C c-golf run
	${MAKE} -C cs run
	${MAKE} -C awk run
.if "${WITH_BASIC}" == "1"
	${MAKE} -C c64-basic run
.endif

run-nobasic:
	${MAKE} -C c run
	${MAKE} -C cpp run
	${MAKE} -C c-golf run
	${MAKE} -C cs run
	${MAKE} -C awk run

clean:
	${MAKE} -C c clean
	${MAKE} -C cpp clean
	${MAKE} -C c-golf clean
	${MAKE} -C cs clean
	${MAKE} -C c64-basic clean

.PHONY: all nobasic run run-nobasic clean
