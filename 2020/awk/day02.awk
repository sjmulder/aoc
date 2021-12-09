function nmatch(s, r) { return gsub(r,"",s) }

BEGIN		{ FS = "[ :-]+" }
nmatch($4, $3) >= $1 && nmatch($4, $3) <= $2 \
		{ part1++ }
xor(substr($4,$1,1)==$3, substr($4,$2,1)==$3) \
		{ part2++ }
END		{ print "02:", part1,  part2 }
