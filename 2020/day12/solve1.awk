function abs(x) { return x<0 ? -x : x }

	{ n = substr($0, 2) }
/F/	{ $1 = substr("ESWN", dir+1, 1) }
/N/	{ y -= n }
/S/	{ y += n }
/W/	{ x -= n }
/E/	{ x += n }
/R/	{ dir = (dir+  n/90) % 4 }
/L/	{ dir = (dir+4-n/90) % 4 }
END	{ print abs(x) + abs(y) }
