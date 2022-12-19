#!/bin/sh

echo "lines:     " $(wc -l $1 | awk '{print $1}')
echo "max length:" $(wc -L $1 | awk '{print $1}')

grep -Eo -- '-?[0-9]+' $1 | awk '
      { if ($0 < min || !have_min) { min = $0; have_min = 1; }
        if ($0 > max || !have_max) { max = $0; have_max = 1; } }
  END { print("min int:   ", min);
	print("max int:   ", max); }'
