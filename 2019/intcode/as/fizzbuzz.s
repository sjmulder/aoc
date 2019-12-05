loop:	add	[n],	1,	[n]0
	eq	[n],	101,	[acc]
	jt	[acc],	end
	eq	[n],	[f_at],	[acc]
	jt	[acc],	fizz
	eq	[n],	[b_at],	[acc]
	jt	[acc],	buzz
	out	[n]
	jt	1,	loop

fizz:	eq	[n],	[b_at],	[acc]
	jt	[acc],	fibu
	out	33333
	add	[f_at],	3,	[f_at]
	jt	1,	loop

buzz:	out	55555
	add	[b_at],	5,	[b_at]
	jt	1,	loop

fibu:	out	88888
	add	[f_at],	3,	[f_at]
	add	[b_at],	5,	[b_at]
	jt	1,	loop

end:	hlt

acc:	0
n:	0
f_at:	3
b_at:	5
fb_at:	15
