#!/bin/sh
echo 01: $(./day01-p1 <../../inputs/2016/01-input.txt) &
echo 02: $(./day02    <../../inputs/2016/02-input.txt) &
echo 03: $(./day03    <../../inputs/2016/03-input.txt) &
./day06 <../../inputs/2016/06-input.txt &
wait
