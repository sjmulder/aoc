./day01    <../data/01-input.txt &
./day02    <../data/02-input.txt &
echo '03': $(./day03-p1 <../data/03-input.txt) \
           $(./day03-p2 <../data/03-input.txt) &
./day04    ckczppom
echo '05:' $(./day05-p1 <../data/05-input.txt) \
           $(./day05-p2 <../data/05-input.txt) &
echo '06:' $(./day06-p1 <../data/06-input.txt) \
           $(./day06-p2 <../data/06-input.txt) &
echo '07:' $(./wiresim a <../data/07-input.txt) \
           $(./wiresim a <../data/07-input-p2.txt) &
./day08    <../data/08-input.txt &
./day09    <../data/09-input.txt &
echo '10:' $(./day10 1113222113 40) \
           $(./day10 1113222113 50) &
echo '11:' $(./day11 cqjxjnds) \
           $(./day11 cqjxxyzz) &
echo '12:' $(./day12-p1 <../data/12-input.txt) \
           $(./day12-p2 <../data/12-input.txt) &
./day13    <../data/13-input.txt &
./day14    <../data/14-input.txt &
./day15    <../data/15-input.txt &
./day16    <../data/16-input.txt &
./day17    <../data/17-input.txt &
wait
