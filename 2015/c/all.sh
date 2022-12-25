./day01    <../../inputs/2015/01-input.txt &
./day02    <../../inputs/2015/02-input.txt &
echo '03': $(./day03-p1 <../../inputs/2015/03-input.txt) \
           $(./day03-p2 <../../inputs/2015/03-input.txt) &
./day04    ckczppom
echo '05:' $(./day05-p1 <../../inputs/2015/05-input.txt) \
           $(./day05-p2 <../../inputs/2015/05-input.txt) &
echo '06:' $(./day06-p1 <../../inputs/2015/06-input.txt) \
           $(./day06-p2 <../../inputs/2015/06-input.txt) &
echo '07:' $(./wiresim a <../../inputs/2015/07-input.txt) \
           $(./wiresim a <../../inputs/2015/07-input-p2.txt) &
./day08    <../../inputs/2015/08-input.txt &
./day09    <../../inputs/2015/09-input.txt &
echo '10:' $(./day10 1113222113 40) \
           $(./day10 1113222113 50) &
echo '11:' $(./day11 cqjxjnds) \
           $(./day11 cqjxxyzz) &
echo '12:' $(./day12-p1 <../../inputs/2015/12-input.txt) \
           $(./day12-p2 <../../inputs/2015/12-input.txt) &
./day13    <../../inputs/2015/13-input.txt &
./day14    <../../inputs/2015/14-input.txt &
./day15    <../../inputs/2015/15-input.txt &
./day16    <../../inputs/2015/16-input.txt &
./day17    <../../inputs/2015/17-input.txt &
./day18    <../../inputs/2015/18-input.txt &
./day19    <../../inputs/2015/19-input.txt &
wait
