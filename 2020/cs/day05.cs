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

		Console.WriteLine(nums.Max());
		Console.WriteLine(nums
			.Zip(nums.Skip(1), (a,b) => (a,b))
			.First(p => p.Item1+2 == p.Item2)
			.Item1 + 1);
	}
}
