#!/bin/sh
echo 01: $(./day01-p1 <../../inputs/2018/01-input.txt) \
         $(./day01-p2 <../../inputs/2018/01-input.txt) &
echo 02: $(./day02-p1 <../../inputs/2018/02-input.txt) \
         $(./day02-p2 <../../inputs/2018/02-input.txt) &
echo 03: $(./day03-p1 <../../inputs/2018/03-input.txt) \
         $(./day03-p2 <../../inputs/2018/03-input.txt) &
#echo 04: $(./day04-p1 <../../inputs/2018/04-input.txt) &  # prints junk
wait
