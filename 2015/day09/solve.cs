using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

static class Program
{
	static HashSet<string> nodes = new HashSet<string>();
	static Dictionary<(string, string), int> costs
		= new Dictionary<(string, string), int>();

	static int Recur(
		int cost, string current,
		HashSet<string> unvisited,
		Func<IEnumerable<int>, int> reduce)
	{
		if (unvisited.Count == 0)
			return cost;

		return reduce(unvisited.Select(dest => Recur(
			cost + costs[(current, dest)], dest,
			new HashSet<string>(unvisited.Where(x => x != dest)),
			reduce)));
	}

	static void Main(string[] args)
	{
		var splitLines = File.ReadAllLines(args[0])
			.Select(l => l.Split(' '));

		foreach (var fields in splitLines)
		{
			nodes.Add(fields[0]); costs[(null, fields[0])] = 0;
			nodes.Add(fields[2]); costs[(null, fields[2])] = 0;
			costs[(fields[0], fields[2])] = int.Parse(fields[4]);
			costs[(fields[2], fields[0])] = int.Parse(fields[4]);
		}

		Console.WriteLine(Recur(0, null, nodes, xs => xs.Min()));
		Console.WriteLine(Recur(0, null, nodes, xs => xs.Max()));
	}
}
