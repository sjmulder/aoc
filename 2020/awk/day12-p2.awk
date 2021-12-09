#!/usr/bin/awk -f

function abs(x) { return x<0 ? -x : x }

BEGIN	{ wx = 10; wy = -1 }

	{ n = substr($0, 2) }
/F/	{ x += wx*n; y += wy*n }
/N/	{ wy -= n }
/S/	{ wy += n }
/W/	{ wx -= n }
/E/	{ wx += n }
/R/	{ r = n }
/L/	{ r = 360-n }

r==90	{ tmp = wx; wx = -wy; wy =  tmp }
r==270	{ tmp = wx; wx =  wy; wy = -tmp }
r==180	{ wx = -wx; wy = -wy; }
	{ r = 0 }

END	{ print "12 (p2):", abs(x) + abs(y) }
