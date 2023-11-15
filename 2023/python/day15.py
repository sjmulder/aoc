#!/usr/bin/env python3

import sys
import re
from collections import defaultdict
from functools import reduce

def hasher(s):
  return reduce(lambda a, c: (a+c) * 17 % 256, [ord(c) for c in s], 0)

p1 = p2 = 0
boxes = defaultdict(dict)

for s in sys.stdin.read().strip().split(","):
  p1 += hasher(s)
  lbl = re.search("\w+", s).group()
  box = hasher(lbl)
  if "=" in s:
    boxes[box][lbl] = int(re.search("\d+", s).group()) 
  elif "-" in s and lbl in boxes[box]:
    del boxes[box][lbl]

for box in boxes:
  for i,lbl in enumerate(boxes[box]):
    p2 += (box+1) * (i+1) * boxes[box][lbl]

print("15:", p1, p2)
