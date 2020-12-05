	{ oldlen = length($0)
	  gsub(/\\\\|\\"|\\x../, "*")
	  acc += oldlen - length($0) +2 }
END	{ print acc }
