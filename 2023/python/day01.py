#!/usr/bin/env python3

import sys
import re

p1 = p2 = 0
names = ["zero", "one", "two", "three", "four", "five", "six", "seven",
         "eight", "nine"]
re2 = "(?=(" + "|".join(names) + "|[0-9]))"
map = {}

for i, name in enumerate(names):
  map[name] = str(i)
for i in range(10):
  map[str(i)] = str(i)


for line in sys.stdin.readlines():
  digits = re.findall("[0-9]", line)
  p1 += int(digits[0] + digits[-1])
  digits = re.findall(re2, line)
  p2 += int(map[digits[0]] + map[digits[-1]])

print("01:", p1, p2)
