using System;
using System.IO;
using System.Linq;

static class Program
{
	static void Main(string[] args)
	{
		var nums = File.ReadAllLines(args[0])
			.Select(line => line.Aggregate(0, (acc, c) =>
				(acc<<1) + (c=='B' || c=='R' ? 1 : 0)))
			.OrderBy(x => x)
			.ToList();

		var p1 = nums.Max();
		var p2 = nums
			.Zip(nums.Skip(1), (a,b) => (a,b))
			.First(p => p.Item1+2 == p.Item2)
			.Item1 + 1;

		Console.WriteLine($"05: {p1} {p2}");
	}
}
