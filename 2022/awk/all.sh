#!/bin/sh
awk -f day01.awk <../data/01-input.txt &
awk -f day02.awk <../data/02-input.txt &
awk -f day04.awk <../data/04-input.txt &
wait
