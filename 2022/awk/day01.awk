$1	{ sum += $1 }
NF==0	{ groupEnd(); }
END	{ groupEnd(); print(top1, top_sum) }

function groupEnd() {
	if (top1 < sum)
		top1 = sum;
	for (i=0; i<3; i++)
		if (top[i] < sum) {
			top_sum += sum - top[2];
			for (j=2; j >= i; j--)
				top[j+1] = top[j];
			top[i] = sum;
			break;
		}
	sum = 0;
}
