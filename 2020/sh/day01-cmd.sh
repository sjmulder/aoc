#!/bin/sh
cmb -fk2-3 -Xa -F2020 input | awk '{print $1*$2*($3||1)}' FS=+
