using System;
using System.IO;
using System.Linq;

class Program
{
    static void Main()
    {
        var ns = File.ReadAllLines("input").Select(int.Parse).ToArray();

        var p1 = from x in ns
                 from y in ns
                 where x + y == 2020
                 select x * y;
        Console.WriteLine(p1.First());

        var p2 = from x in ns
                 from y in ns
                 from z in ns
                 where x + y + z == 2020
                 select x * y * z;
        Console.WriteLine(p2.First());
    }
}
