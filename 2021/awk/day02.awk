# d is pt.2 depth, a is pt.1 depth AND aim
/down/		{ a += $2 }
/up/		{ a -= $2 }
/forward/	{ h += $2; d += a*$2; }
END		{ print h*a, h*d }
