#!/bin/sh
set -e
gawk -f ./day02.awk ../../inputs/2020/02-input.txt &
gawk -f ./day03-p1.awk ../../inputs/2020/03-input.txt &
gawk -f ./day12-p1.awk ../../inputs/2020/12-input.txt &
gawk -f ./day12-p2.awk ../../inputs/2020/12-input.txt &
gawk -f ./day13-p1.awk ../../inputs/2020/13-input.txt &
gawk -f ./day13-p2.awk ../../inputs/2020/13-input.txt &
gawk -f ./day14-p1.awk ../../inputs/2020/14-input.txt &
wait
