set -e
gawk -f ./day02.awk ../data/02-input.txt &
gawk -f ./day03-p1.awk ../data/03-input.txt &
gawk -f ./day12-p1.awk ../data/12-input.txt &
gawk -f ./day12-p2.awk ../data/12-input.txt &
gawk -f ./day13-p1.awk ../data/13-input.txt &
gawk -f ./day13-p2.awk ../data/13-input.txt &
gawk -f ./day14-p1.awk ../data/14-input.txt &
wait
