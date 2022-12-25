#!/bin/sh
awk -f day01.awk <../../inputs/2022/01-input.txt &
awk -f day02.awk <../../inputs/2022/02-input.txt &
awk -f day04.awk <../../inputs/2022/04-input.txt &
wait
