#!/bin/sh
echo 01: $(./day01-p1 <../data/01-input.txt) &
echo 02: $(./day02    <../data/02-input.txt) &
echo 03: $(./day03    <../data/03-input.txt) &
wait
