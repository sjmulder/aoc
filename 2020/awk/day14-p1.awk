#!/usr/bin/awk -f

/mask/	{ mask_and = mask_or = 0
	  len = length($3)
	  for (i = 1; i <= len; i++) {
		c = substr($3, i, 1);
		mask_and = (mask_and *2) + (c == "X")
		mask_or  = (mask_or  *2) + (c == "1")
	  } }
/mem/	{ match($1, /[0-9]+/, matches)
	  mem[matches[0]] = or(and($3, mask_and), mask_or) }
END	{ sum = 0
	  for (addr in mem)
		sum += mem[addr]
	  print "14 (p1):", sum }
