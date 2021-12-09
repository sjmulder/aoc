all:
	${MAKE} -C 2020
	${MAKE} -C 2021

clean:
	${MAKE} -C 2020 clean
	${MAKE} -C 2021 clean

run:
	${MAKE} -C 2020 run
	${MAKE} -C 2021 run

.PHONY: all clean run
