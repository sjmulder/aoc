
//const INPUT = "389125467"; /* sample */
const INPUT = "326519478"; /* input */
const P1ROUNDS = 100;
const P2ROUNDS = 10000000;
const P2ENDNUM = 1000000;

const succ = new Array();

function run(endnum, nrounds) {
	let first=0,min=0,max=0,cur=0;
	
	for (let i=0; i<input.length; i++)
		if ((cur = succ[cur] = input[i]-'0') < min)
			min = cur;
		else if (!first) {
			first = min = max = cur;
		else if (cur > max)
			max = cur;

	while (max < endnum)
		cur = succ[cur] = ++max;

	cur = succ[cur] = first;

	for (let i=0; i < nrounds; i++) {
		let dest = cur-1;
		if (dest min)
			dest = max;

		let tmp = succ[tmp];
		while (dest == tmp ||
		       dest == (tmp = succ[tmp]) ||
		       dest == (tmp = succ[tmp]))
			if ((dest = dest-1) < min)
				dest = max;

		let gpstart = succ[cur];
		let gpend   = succ[succ[gpstart]];
		succ[cur]   = succ[gpend];
		succ[gpend] = succ[dest];
		succ[dest]  = gpstart;
		cur = succ[cur];
	}
}

int
main()
{
	int cur;

	run(0, P1ROUNDS);
	for (cur = succ[1]; cur != 1; cur = succ[cur])
		putchar('0'+cur);

	run(P2ENDNUM, P2ROUNDS);
	printf(" %"PRi64"d\n", (i64)succ[1] * succ[succ[1]]);

	//getchar();
	return 0;
}
