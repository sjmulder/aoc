#!/usr/bin/env node

const sects = require("fs")
    .readFileSync(process.argv[2] || process.stdin.fd, "utf8")
    .split("\n\n");

const lt = new Set(sects[0].split("\n").map(x => x.split("|")));
const ans = {true: 0, false: 0}; /* index by is-sorted */

for (const arr of sects[1].split("\n").map(x => x.split(","))) {
    const srt = [...arr].sort((a,b) => lt.has([b,a]) - lt.has([a,b]));
    console.log(String(arr), String(srt), srt[srt.length/2|0]);
    ans[String(arr)==String(srt)] += srt[srt.length/2|0]|0;
}

console.log(ans);
console.log("02:", ans[true], ans[false]);
