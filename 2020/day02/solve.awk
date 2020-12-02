function xxor(a, b) { return a && !b || !a && b }

function nmatch(s, r) {
	for (n=0; p = match(s, r); n++)
		s = substr(s, p+1);
	return n;
}

BEGIN		{ FS = "[ :-]+" }
nmatch($4, $3) >= $1 && nmatch($4, $3) <= $2 \
		{ part1++ }
xxor(substr($4,$1,1)==$3, substr($4,$2,1)==$3) \
		{ part2++; print NR " match" }
END		{ print part1 " " part2 }
