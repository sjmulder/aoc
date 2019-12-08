using System;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Aoc.Util;

class Program
{
    const int W = 25;
    const int H = 6;

    static async Task Main(string[] args)
    {
        var layers = (await File
            .ReadAllTextAsync("input"))
            .TrimEnd('\n')
            .Slice(W * H)
            .Select(cs => new String(cs))
            .ToArray();

        var minLayer = layers
            .MinBy(l => l.Count(c => c == '0'));
        int part1 =
            minLayer.Count(c => c == '1') *
            minLayer.Count(c => c == '2');

        Console.WriteLine($"Part 1: {part1}");

        var image = layers
            .Aggregate(
                Enumerable.Repeat('2', W * H),
                (acc, l) => acc
                    .Zip(l)
                    .Select(p => p.First == '2' ? p.Second : p.First))
            .Select(c => c == '1' ? '#' : ' ')
            .Slice(W)
            .Select(cs => new String(cs))
            .ToArray();

        Console.WriteLine($"Part 2:");

        foreach (var line in image)
            Console.WriteLine(line);
    }
}
