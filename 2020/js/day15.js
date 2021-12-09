#!/usr/bin/node

const fs = require('fs');

const map = new Map();
const input = fs.readFileSync('input', 'utf-8').split(',');
let part1, cur, prev=0 ;

for (let turn=0; turn < 30000000; turn++) {
	if (turn < input.length)
		cur = parseInt(input[turn], 10);
	else if ((cur = map.get(prev) || 0))
		cur = turn - cur;
	map.set(prev, turn);
	prev = cur;
	if (turn == 2019) part1 = cur;
}

console.log("15:", part1, cur);
