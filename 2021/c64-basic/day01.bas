#!/usr/bin/env cbmbasic
10 OPEN 1,1,0,"data/01-input.txt"
20 INPUT#1,A:N=N+1
30 IF ST>0 THEN PRINT "01:";P1;P2:END
40 IF N>1 AND A>B THEN P1=P1+1
50 IF N>3 AND A>D THEN P2=P2+1
60 D=C:C=B:B=A:GOTO 20
