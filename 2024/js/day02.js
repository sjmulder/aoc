#!/usr/bin/env node

const fs = require("fs");

const indices = xs => [...xs.keys()];
const countBy = (xs, pred) => xs.reduce(
    (acc, x) => acc += pred(x) ? 1 : 0, 0);

const someBy = (xs, pred) => {
    for (const x of xs) {
        if (pred(x)) {
            return true;
        }
    }
    return false;
};

const pairwise = xs => {
    const pairs = [];
    for (let i=1; i < xs.length; i++) {
        pairs.push([xs[i-1], xs[i]]);
    }
    return pairs;
};

const isSafe = xs =>
    pairwise(xs).every(([a,b]) => a!==b && a-b > -4 && a-b < 4) &&
    new Set(pairwise(xs).map(([a,b]) => a < b)).size === 1;

const rows = fs
    .readFileSync(process.argv[2] || process.stdin.fd, "utf8")
    .split("\n")
    .filter(x => x != "")
    .map(x => x.split(/ +/).map(Number));

const p1 = countBy(rows, isSafe);
const p2 = countBy(rows, row =>
    isSafe(row) || someBy(indices(row),
        i => isSafe([...row.slice(0, i), ...row.slice(i+1)])));

console.log("01:", p1, p2);
