#!/usr/bin/awk -f

BEGIN		{ RS="[,\n]"; bm=9999 }
NR==1		{ t = $1 }
NR!=1 && /[^x]/	{ m = $1-t%$1; if (m<bm) { bm=m; bid=$1 } }
END		{ print "13 (p1):", bm * bid }
