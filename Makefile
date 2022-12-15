all:
	${MAKE} -C 2015
	${MAKE} -C 2020
	${MAKE} -C 2021
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

.PHONY: all clean run
