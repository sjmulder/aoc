substr($0, (NR-1)*3 % length($0) +1, 1) == "#" { n++ }
END { print "03 (p1):", n }
