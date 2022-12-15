all:
	${MAKE} -C c
	${MAKE} -C c-golf
	${MAKE} -C cs

clean:
	${MAKE} -C c clean
	${MAKE} -C c-golf clean
	${MAKE} -C cs clean

run:
	${MAKE} -C awk run
	${MAKE} -C c run
	${MAKE} -C c-golf run
	${MAKE} -C cs run
	${MAKE} -C js run
.if "${WITH_BASIC}" == "1"
	${MAKE} -C c64-basic run
.endif

.PHONY: all run clean
