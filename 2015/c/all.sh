./day01    <../data/01-input.txt &
./day02i   <../data/02-input.txt &
./day03-p1 <../data/03-input.txt &
./day03-p2 <../data/03-input.txt &
./day04
./day05-p1 <../data/05-input.txt &
./day05-p2 <../data/05-input.txt &
./day06-p1 <../data/06-input.txt &
./day06-p2 <../data/06-input.txt &
echo '07:' $(./wiresim a <../data/07-input.txt) \
           $(./wiresim a <../data/07-input-p2.txt) &
echo '08: not implemented'
echo '09: not implemented'
echo '10:' $(./day10 cqjxjnds 40) \
           $(./day10 cqjxjnds 50) &
./day11    cqjxjnds
./day12-p1 <../data/12-input.txt &
./day12-p2 <../data/12-input.txt &
./day13    <../data/13-input.txt &
./day14    <../data/14-input.txt &
echo '15: unfinished'
wait
