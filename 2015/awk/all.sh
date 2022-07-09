echo '08:' $(awk -f day08-p1.awk ../data/08-input.txt) \
           $(awk -f day08-p2.awk ../data/08-input.txt) &
wait
