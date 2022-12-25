#!/bin/sh

echo 01: `./day01 ../../inputs/2020/01-input.txt` &
echo 02: `./day02 ../../inputs/2020/02-input.txt` &
echo 03: `./day03 ../../inputs/2020/03-input.txt` &
echo 04: `./day04 ../../inputs/2020/04-input.txt` &
echo 05: `./day05 ../../inputs/2020/05-input.txt` &
echo 06: `./day06 ../../inputs/2020/06-input.txt` &
echo 07: `./day07 ../../inputs/2020/07-input.txt` &
echo 08: `./day08 ../../inputs/2020/08-input.txt` &
echo 09: `./day09 ../../inputs/2020/09-input.txt` &
echo 10: `./day10 ../../inputs/2020/10-input.txt` &
echo 11: `./day11 ../../inputs/2020/11-input.txt` &
echo 12: `./day12 ../../inputs/2020/12-input.txt` &
echo 13: `./day13 ../../inputs/2020/13-input.txt` &
echo 14: `./day14 ../../inputs/2020/14-input.txt` &
echo 15: `./day15 ../../inputs/2020/15-input.txt` &
echo 16: `./day16 ../../inputs/2020/16-input.txt` &
echo 17: `./day17 ../../inputs/2020/17-input.txt` &
echo 18: `./day18 ../../inputs/2020/18-input.txt` &
echo 19: `./day19 ../../inputs/2020/19-input.txt` \
         `./day19 ../../inputs/2020/19-p2-input.txt` &
echo 20: `./day20 ../../inputs/2020/20-input.txt` &
echo 21: `./day21 ../../inputs/2020/21-input.txt` &
echo 22: `./day22 ../../inputs/2020/22-input.txt` &
echo 23: `./day23` &
echo 24: `./day24 ../../inputs/2020/24-input.txt` &
echo 25: `./day25` &

wait
