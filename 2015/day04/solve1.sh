#!/bin/sh

# Doesn't work on my WSL, 'cannot fork'
i=1
while ! echo -n $1$i | md5sum | grep -q '^00000'; do
  echo $1$i >&2
  i=`expr $i + 1`
done
echo "FOUND: %d"
