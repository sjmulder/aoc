#!/usr/bin/env python3

import sys
from collections import defaultdict

lines = sys.stdin.readlines()
gear_nums = defaultdict(list)
p1 = p2 = 0

for (y, line) in enumerate(lines):
  num = 0
  sym = False
  gears = set()
  for (x, c) in enumerate(line):
    if c.isdigit():
      num = num*10 + int(c)
      for y2 in [y-1, y, y+1]:
        if y2 < 0: continue
        if y2 >= len(lines): continue
        for x2 in [x-1, x, x+1]:
          if x2 < 0: continue
          if x2 >= len(lines[y2]): continue
          c = lines[y2][x2]
          if not c.isdigit() and c != '.':
            sym = True
            if c == '*':
              gears.add((x2, y2))
    elif num > 0:
      if sym:
        p1 += num
        for gear in gears:
          gear_nums[gear].append(num)
      num = 0
      sym = False
      gears = set()

for nums in gear_nums.values():
  if len(nums) == 2:
    p2 += nums[0] * nums[1]

print("03:", p1, p2)
