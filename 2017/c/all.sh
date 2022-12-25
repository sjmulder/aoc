#!/bin/sh
echo 01: $(./day01-p1 <../../inputs/2017/01-input.txt) \
         $(./day01-p2 <../../inputs/2017/01-input.txt) & 
echo 02: $(./day02-p1 <../../inputs/2017/02-input.txt) \
         $(./day02-p2 <../../inputs/2017/02-input.txt) & 
echo 03: $(./day03-p1 325489) \
         $(./day03-p2 325489) & 
echo 04: $(./day04-p1 <../../inputs/2017/04-input.txt) \
         $(./day04-p2 <../../inputs/2017/04-input.txt) & 
echo 08: $(./day08 <../../inputs/2017/08-input.txt) &
echo 09: $(./day09 <../../inputs/2017/09-input.txt) &
echo 10: $(./day10-p1 <../../inputs/2017/10-input.txt) \
         $(./day10-p2 <../../inputs/2017/10-input.txt) & 
echo 11: $(./day11-p1 <../../inputs/2017/11-input.txt) \
         $(./day11-p2 <../../inputs/2017/11-input.txt) & 
#echo 12: $(./day12-p1 0 <../../inputs/2017/12-input.txt) \  # prints junk
#         $(./day12-p2   <../../inputs/2017/12-input.txt) &  # prints junk
echo 13: $(./day13-p1 <../../inputs/2017/13-input.txt) \
         $(./day13-p2 <../../inputs/2017/13-input.txt) & 
#echo 14: $(./day14-p1 amgozmfv) \  # prints junk
#         $(./day14-p2 amgozmfv) &  # prints junk
echo 15: $(./day15-p1) $(./day15-p2) & 
#echo 20: $(./day20-p1 <../../inputs/2017/20-input.txt) &  # prints junk
wait
