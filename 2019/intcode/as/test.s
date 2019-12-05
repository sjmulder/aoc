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

	jt	0,	fail
	jt	0,	[failp]
	jt	[zero],	fail
	jt	[zero],	[failp]

	jf	1,	fail
	jf	1,	[failp]
	jf	[one],	fail
	jf	[one],	[failp]

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
