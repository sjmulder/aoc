#!/usr/bin/env python3
import sys
import re
from networkx import Graph
from networkx.algorithms.community import greedy_modularity_communities

G = Graph()

for line in sys.stdin.readlines():
  words = re.findall("\\w+", line)
  for to in words[1:]:
    G.add_edge(words[0], to)

coms = greedy_modularity_communities(G, best_n=2)

print("25:", len(coms[0]) * len(coms[1]))
