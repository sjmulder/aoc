BEGIN {
	print("int")
	print("monad(int input[14]) {")
	print("\tint w=0,x=0,z=0,y=0, i=0;")
	print("")
}

/inp/	{ print("\t" $2 " = input[i++];") }
/add/	{ print("\t" $2 " += " $3 ";") }
/mul/	{ print("\t" $2 " *= " $3 ";") }
/div/	{ print("\t" $2 " /= " $3 ";") }
/mod/	{ print("\t" $2 " %= " $3 ";") }
/eql/	{ print("\t" $2 " = " $2 " == " $3 ";") }

END {
	print("")
	print("\treturn w;")
	print("}")
}
