all:
	${MAKE} -C c
	${MAKE} -C c-golf
	${MAKE} -C cs

nobasic: all

clean:
	${MAKE} -C c clean
	${MAKE} -C c-golf clean
	${MAKE} -C cs clean

run:
	${MAKE} -C awk run
	${MAKE} -C c run
	${MAKE} -C c-golf run
	${MAKE} -C c64-basic run
	${MAKE} -C cs run
	${MAKE} -C js run

run-nobasic:
	${MAKE} -C awk run
	${MAKE} -C c run
	${MAKE} -C c-golf run
	${MAKE} -C cs run
	${MAKE} -C js run

.PHONY: all nobasic run run-nobasic clean
