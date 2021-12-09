set -e

echo 01: `./day01 ../data/01-input.txt` &
echo 02: `./day02 ../data/02-input.txt` &
echo 03: `./day03 ../data/03-input.txt` &
echo 04: `./day04 ../data/04-input.txt` &
echo 05: `./day05 ../data/05-input.txt` &
echo 06: `./day06 ../data/06-input.txt` &
echo 07: `./day07 ../data/07-input.txt` &
echo 08: `./day08 ../data/08-input.txt` &
echo 09: `./day09 ../data/09-input.txt` &
echo 10: `./day10 ../data/10-input.txt` &
echo 11: `./day11 ../data/11-input.txt` &
echo 12: `./day12 ../data/12-input.txt` &
echo 13: `./day13 ../data/13-input.txt` &
echo 14: `./day14 ../data/14-input.txt` &
echo 15: `./day15 ../data/15-input.txt` &
echo 16: `./day16 ../data/16-input.txt` &
echo 17: `./day17 ../data/17-input.txt` &
echo 18: `./day18 ../data/18-input.txt` &
echo 19: `./day19 ../data/19-input.txt` \
         `./day19 ../data/19-p2-input.txt` &
echo 20: `./day20 ../data/20-input.txt` &
echo 21: `./day21 ../data/21-input.txt` &
echo 22: `./day22 ../data/22-input.txt` &
echo 23: `./day23` &
echo 24: `./day24 ../data/24-input.txt` &
echo 25: `./day25` &

wait
