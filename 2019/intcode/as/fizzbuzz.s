loop:
	add [n], 1, [n]
	eq n:0, 101, [is_done]; jt is_done:0, [done]

	eq [n], next_fibu:15, [is_fibu]; jt is_fibu:0, fibu
	eq [n], next_fizz: 3, [is_fizz]; jt is_fizz:0, fizz
	eq [n], next_buzz: 5, [is_buzz]; jt is_buzz:0, buzz
	out [n]
	jt 1, [loop]

fibu:
	out 88888
	add [next_fibu], 15, [next_fibu]
	add [next_fizz],  3, [next_fizz]
	add [next_buzz],  5, [next_buzz]
	jt 1, loop

fizz:
	out 33333
	add [next_fizz], 3, [next_fizz]
	jt 1, loop

buzz:
	out 55555
	add [next_buzz], 5, [next_buzz]
	jt 1, loop

done:
	hlt
