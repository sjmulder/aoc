#!/usr/bin/env cbmbasic
10 OPEN 1,1,0,"data/02-input.txt"
20 IF ST>0 THEN PRINT H*A,H*D:END
30 INPUT#1,DIR$,X
40 IF DIR$="down" THEN A=A+X:GOTO 20
50 IF DIR$="up" THEN A=A-X:GOTO 20
60 IF DIR$="forward" THEN H=H+X:D=D+(A*X):GOTO 20
70 PRINT "BAD INPUT:",DIR$:END
