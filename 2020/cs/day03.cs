using System;
using System.IO;
using System.Linq;

class Program {
	static void Main(string[] args) {
		var ls = File.ReadAllLines(args[0]);
		int w = ls[0].Length;
		Func<int,int,long> f = (dx, dy) => ls
			.Where((l,i) => i%dy == 0 && l[i*dx/dy%w] == '#')
			.Count();
		Console.WriteLine(f(3,1));
		Console.WriteLine(f(1,1)*f(3,1)*f(5,1)*f(7,1)*f(1,2));
	}
}
