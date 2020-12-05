using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

static class Extensions
{
    static int Product(this IEnumerable<int> xs) => xs.Aggregate(1, (a, x) => a * x);

    static IEnumerable<IEnumerable<T>> Combine<T>(this T[] xs, int n)
    {
        if (n > xs.Length) yield break;
        var idxs = Enumerable.Range(0, n).ToArray();

        while (true)
        {
            yield return idxs.Select(ix => xs[ix]);
            int i = n-1;
            while (++idxs[i] > xs.Length-n+i) if (--i < 0) yield break;
            while (++i < n) idxs[i] = idxs[i-1]+1;
        }
    }

    static void Main()
    {
        var nrs = File.ReadAllLines("input").Select(int.Parse).ToArray();
        Console.WriteLine(nrs.Combine(2).First(x => x.Sum() == 2020).Product());
        Console.WriteLine(nrs.Combine(3).First(x => x.Sum() == 2020).Product());
    }
}
