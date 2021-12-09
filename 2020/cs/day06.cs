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
		var p1 = gs.Sum(g => az.Count(c => g.Any(s => s.Contains(c))));
		var p2 = gs.Sum(g => az.Count(c => g.All(s => s.Contains(c))));
		Console.WriteLine($"06: {p1} {p2}");
	}
}
