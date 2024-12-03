#!/usr/bin/env node

const fs = require("fs");
const U = require("./util");

const isSafe = xs =>
    U.pairwise(xs).every(([a,b]) => a!==b && a-b > -4 && a-b < 4) &&
    new Set(U.pairwise(xs).map(([a,b]) => a < b)).size === 1;

const rows = fs
    .readFileSync(process.argv[2] || process.stdin.fd, "utf8")
    .split("\n")
    .filter(x => x != "")
    .map(x => x.split(/ +/).map(Number));

const p1 = U.countBy(rows, isSafe);
const p2 = U.countBy(rows, row =>
    isSafe(row) || U.someBy(U.indices(row),
        i => isSafe([...row.slice(0, i), ...row.slice(i+1)])));

console.log("01:", p1, p2);
