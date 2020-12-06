using System;
using System.IO;
using System.Linq;

class Program
{
	static void Main(string[] args)
	{
		var az = "abcdefghijklmnopqrstuvwxyz";
		var gs = File.ReadAllText(args[0]).Split("\n\n")
			.Select(gp => gp.Split('\n').Where(g => g != ""));
		Console.WriteLine(gs.Sum(g => az.Count(c => g.Any(s => s.Contains(c)))));
		Console.WriteLine(gs.Sum(g => az.Count(c => g.All(s => s.Contains(c)))));
	}
}
