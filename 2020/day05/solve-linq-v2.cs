using System;
using System.IO;
using System.Linq;

static class Program
{
	static IEnumerable<T> WhereZip<T, U>(
			this IEnumerable<T> xs,
			IEnumerable<U> ys,
			Func<T, U, bool> pred)
		=> xs.Zip(ys, (x,y) => (x,y))
			.Where(p => pred(p.Item1, p.Item2))
			.Select(p => p.Item1);

	static IEnumerable<T> FirstZip<T, U>(
			this IEnumerable<T> xs,
			IEnumerable<U> ys,
			Func<T, U, bool> pred)
		=> xs.WhereZip(ys).First();

	static void Main(string[] args)
	{
		var nums = File.ReadAllLines(args[0])
			.Select(line => line.Aggregate(0, (acc, c) =>
				(acc<<1) + (c=='B' || c=='R' ? 1 : 0)))
			.OrderBy(x => x)
			.ToList();

		Console.WriteLine(nums.Max());
		Console.WriteLine(nums.FirstZip(nums.Skip(1), (a,b) => a+2==b);
	}
}
