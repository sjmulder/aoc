#!/usr/bin/env cbmbasic
0 Z=0: L=179: R=201: B=-109: T=-63
1 FOR M=FLOOR(SQR(L)/2) TO R: X=Z: U=M
2  X=X+U: U=U-1: IF X<L THENIF U THEN 2
3  IF X<L OR X>R THEN 9
4  FOR N=B TO -B: X=Z: Y=Z: U=M: V=N
5   Y=Y+V: V=V-1: X=X+U: IF U THEN U=U-1
6   IF X<L OR Y>T THENIF X<=R THENIF Y>=B THEN 5
7   IF X>=L THENIF X<=R THENIF Y>=B THENIF Y<=T THEN A=A+1
8  NEXT N
9 NEXT M: PRINT "17:";B*(B+1)/2;A
