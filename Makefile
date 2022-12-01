all:
	${MAKE} -C 2015
	${MAKE} -C 2020
	${MAKE} -C 2021
	${MAKE} -C 2022

nobasic:
	${MAKE} -C 2015
	${MAKE} -C 2020 nobasic
	${MAKE} -C 2021 nobasic
	${MAKE} -C 2022

clean:
	${MAKE} -C 2015 clean
	${MAKE} -C 2020 clean
	${MAKE} -C 2021 clean
	${MAKE} -C 2022 clean

run:
	${MAKE} -C 2015 run
	${MAKE} -C 2020 run
	${MAKE} -C 2021 run
	${MAKE} -C 2022 run

run-nobasic:
	${MAKE} -C 2015 run
	${MAKE} -C 2020 run-nobasic
	${MAKE} -C 2021 run-nobasic
	${MAKE} -C 2022 run

.PHONY: all nobasic clean run-nobasic
