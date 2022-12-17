#!/bin/sh
echo 01: $(./day01-p1 <../data/01-input.txt) \
         $(./day01-p2 <../data/01-input.txt) & 
echo 02: $(./day02-p1 <../data/02-input.txt) \
         $(./day02-p2 <../data/02-input.txt) & 
echo 03: $(./day03-p1 325489) \
         $(./day03-p2 325489) & 
echo 04: $(./day04-p1 <../data/04-input.txt) \
         $(./day04-p2 <../data/04-input.txt) & 
echo 08: $(./day08 <../data/08-input.txt) &
echo 09: $(./day09 <../data/09-input.txt) &
echo 10: $(./day10-p1 <../data/10-input.txt) \
         $(./day10-p2 <../data/10-input.txt) & 
echo 11: $(./day11-p1 <../data/11-input.txt) \
         $(./day11-p2 <../data/11-input.txt) & 
#echo 12: $(./day12-p1 0 <../data/12-input.txt) \  # prints junk
#         $(./day12-p2   <../data/12-input.txt) &  # prints junk
echo 13: $(./day13-p1 <../data/13-input.txt) \
         $(./day13-p2 <../data/13-input.txt) & 
#echo 14: $(./day14-p1 amgozmfv) \  # prints junk
#         $(./day14-p2 amgozmfv) &  # prints junk
echo 15: $(./day15-p1) $(./day15-p2) & 
#echo 20: $(./day20-p1 <../data/20-input.txt) &  # prints junk
wait
