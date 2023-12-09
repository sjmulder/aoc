#!/usr/bin/env python3

import sys

p1 = p2 = 0

for game, line in enumerate(sys.stdin.readlines()):
  rounds = [
    {p[1]: int(p[0]) for p in [c.split() for c in r.split(",")]}
    for r in line.strip().split(":")[1].split(";")
  ]

  r = max([x.get("red", 0) for x in rounds])
  g = max([x.get("green", 0) for x in rounds])
  b = max([x.get("blue", 0) for x in rounds])

  if r <= 12 and g <= 13 and b <= 14:
    p1 += game + 1

  p2 += r * g * b

print("02:", p1, p2)
