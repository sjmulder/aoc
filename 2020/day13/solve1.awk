#!/usr/bin/awk -f

BEGIN		{ RS=","; bm=9999 }
NR==1		{ t = $1 }
NR!=1 && /[^x]/	{ m = $1-t%$1; if (m<bm) { bm=m; bid=$1 } }
END		{ print bm * bid }
