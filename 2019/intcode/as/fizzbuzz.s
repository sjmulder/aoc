loop:
	add 1, [n], [n]
	eq n:0, 101, [is_done]; jt is_done:0, done

	eq next_fibu:15, [n], [is_fibu]; jt is_fibu:0, fibu
	eq next_fizz: 3, [n], [is_fizz]; jt is_fizz:0, fizz
	eq next_buzz: 5, [n], [is_buzz]; jt is_buzz:0, buzz
	out [n]
	jt 1, loop

fibu:
	out 88888
	add 15, [next_fibu], [next_fibu]
	add  3, [next_fizz], [next_fizz]
	add  5, [next_buzz], [next_buzz]
	jt 1, loop

fizz:
	out 33333
	add 3, [next_fizz], [next_fizz]
	jt 1, loop

buzz:
	out 55555
	add 5, [next_buzz], [next_buzz]
	jt 1, loop

done:
	hlt
