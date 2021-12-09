using System;
using System.IO;
using System.Linq;

class Program
{
	static void Main(string[] args)
	{
		var nums = File.ReadAllLines(args[0])
			.Select(int.Parse)
			.ToList();
		nums.Insert(0, 0);
		nums.Add(nums.Max() + 3);
		nums.Sort();

		Console.WriteLine("10 (p1): " + nums
			.Zip(nums.Skip(1), (a, b) => b-a)
			.Where(d => d==1 || d==3)
			.GroupBy(d => d)
			.Select(g => g.Count())
			.Aggregate(1, (acc, x) => acc*x));
	}
}
