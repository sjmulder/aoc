	{ n += gsub(/\\|\"/, "")+2 }
END	{ print n }
