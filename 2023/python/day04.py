#!/usr/bin/env python3

import sys

p1 = p2 = 0
nextra = {}

for (card, line) in enumerate(sys.stdin.readlines()):
  wins, nums = [
    [int(n) for n in s.split()]
    for s in line.split(":")[1].split("|")
  ]
  nmatch = len(set(wins) & set(nums))
  for i in range(nmatch):
    nextra[card+i+1] = nextra.get(card+i+1, 0) + nextra.get(card, 0) + 1
  p1 += int(2 ** (nmatch - 1))
  p2 += nextra.get(card, 0) + 1

print("04:", p1, p2)
