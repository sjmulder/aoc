PROGS+=	solve

solve: solve.o part1.o part2.o
	${CC} ${LDFLAGS} -o solve solve.o part1.o part2.o ${LDLIBS}

part1.o: ../compat/stdint.h
part2.o: ../compat/stdint.h
part2.o: ../compat/builtin.h
solve.o: ../compat/stdint.h
solve.o: ../compat/inttypes.h

include ../common.mk
