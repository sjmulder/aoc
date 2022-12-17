#!/bin/sh
echo 01: $(./day01-p1 <../data/01-input.txt) \
         $(./day01-p2 <../data/01-input.txt) &
echo 02: $(./day02-p1 <../data/02-input.txt) \
         $(./day02-p2 <../data/02-input.txt) &
echo 03: $(./day03-p1 <../data/03-input.txt) \
         $(./day03-p2 <../data/03-input.txt) &
#echo 04: $(./day04-p1 <../data/04-input.txt) &  # prints junk
wait
