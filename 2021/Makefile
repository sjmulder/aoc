all:
	${MAKE} -C c
	${MAKE} -C cs
	${MAKE} -C c64-basic

nobasic:
	${MAKE} -C c
	${MAKE} -C cs

run:
	${MAKE} -C c run
	${MAKE} -C cs run
	${MAKE} -C awk run
	${MAKE} -C c64-basic run

run-nobasic:
	${MAKE} -C c run
	${MAKE} -C cs run
	${MAKE} -C awk run

clean:
	${MAKE} -C c clean
	${MAKE} -C cs clean
	${MAKE} -C c64-basic clean

.PHONY: all nobasic run run-nobasic clean
