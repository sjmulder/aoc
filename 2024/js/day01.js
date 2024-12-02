#!/usr/bin/env node

const fs = require("fs");

const sum = xs => xs.reduce((a, b) => a + b);
const count = (xs, val) => xs.reduce(
    (acc, x) => acc += x == val ? 1 : 0, 0);

const pairs = fs
    .readFileSync(process.argv[2] || process.stdin.fd, "utf8")
    .split("\n")
    .filter(x => x != "")
    .map(x => x.split(/ +/).map(Number));

const ls = pairs.map(x => x[0]); ls.sort();
const rs = pairs.map(x => x[1]); rs.sort();

const p1 = sum(ls.map((l, i) => Math.abs(l - rs[i])));
const p2 = sum(ls.map(l => l * count(rs, l)));

console.log("01:", p1, p2);
