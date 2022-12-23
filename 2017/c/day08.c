#define _CRT_SECURE_NO_WARNINGS
#define MAX(a,b) ((a)>(b)?(a):(b))

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

int
main()
{
	int *regs, val, cval, max=INT_MIN, i;
	uint32_t reg, op, creg, cop;

	if (!(regs = calloc(0x1000000, sizeof(*regs)))) {
		perror(NULL);
		return 1;
	}

	reg = op = creg = cop = 0;
	while (scanf(" %3s %3s %d if %3s %2s %d",
			(char*)&reg, (char*)&op, &val,
			(char*)&creg, (char*)&cop, &cval) == 6) {
		switch (cop) {
		case 0x003C: if (regs[creg] >= cval) goto skip; break;
		case 0x3D3C: if (regs[creg] >  cval) goto skip; break;
		case 0x3D3D: if (regs[creg] != cval) goto skip; break;
		case 0x3D21: if (regs[creg] == cval) goto skip; break;
		case 0x3D3E: if (regs[creg] <  cval) goto skip; break;
		case 0x003E: if (regs[creg] <= cval) goto skip; break;
		default:
			fprintf(stderr, "bad operator: %s (0x%06x)\n",
				(char*)&cop, cop);
			return 1;
		}

		switch (op) {
		case 0x636E69: regs[reg] += val; break;
		case 0x636564: regs[reg] -= val; break;
		default:
			fprintf(stderr, "bad instruction: %s (0x%06x)\n",
				(char*)&op, op);
			return 1;
		}

		/* printf("[0x%06x] = %d\n", reg, regs[reg]); */
		max = MAX(max, regs[reg]);
	skip:
		reg = op = creg = cop = 0;
	}

	printf("max ever:   %d\n", max);

	max = SHRT_MIN;
	for (i = 0; i < 0x1000000; i++)
		max = MAX(max, regs[i]);		

	printf("max at end: %d\n", max);

	return 0;
}
