	add	var0,	var1,	[var2]
	add	var0,	[var1],	[var2]
	add	[var0],	var1,	[var2]
	add	[var0],	[var1],	[var2]

	mul	var0,	var1,	[var2]
	mul	var0,	[var1],	[var2]
	mul	[var0],	var1,	[var2]
	mul	[var0],	[var1],	[var2]

	in	[var0]
	out	[var0]
	out	var0

	jnz	0,	fail
	jnz	0,	[failp]
	jnz	[zero],	fail
	jnz	[zero],	[failp]

	jz	1,	fail
	jz	1,	[failp]
	jz	[one],	fail
	jz	[one],	[failp]

	lt	var0,	var1,	[var2]
	lt	var0,	[var1],	[var2]
	lt	[var0],	var1,	[var2]
	lt	[var0],	[var1],	[var2]

	eq	var0,	var1,	[var2]
	eq	var0,	[var1],	[var2]
	eq	[var0],	var1,	[var2]
	eq	[var0],	[var1],	[var2]

	hlt

var0:	0
var1:	0
var2:	0

zero:	0
one:	1

fail:	99999
failp:	fail
