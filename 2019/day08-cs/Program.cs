using System;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Aoc.Util;

class Program
{
    const int WIDTH = 25;
    const int HEIGHT = 6;

    static async Task Main(string[] args)
    {
        var layers = (await File
            .ReadAllTextAsync("input"))
            .TrimEnd('\r', '\n')
            .Slice(WIDTH)
            .Slice(HEIGHT)
            .ToArray();

        var minLayer = layers
            .MinBy(l => l.Flatten().Count('0'));
        int part1 =
            minLayer.Flatten().Count('1') *
            minLayer.Flatten().Count('2');

        Console.WriteLine($"Part 1: {part1}");

        var image = Enumerable.Range(0, HEIGHT)
            .Select(row => Enumerable.Range(0, WIDTH)
                .Select(col => layers
                    .Select(layer => layer[row][col])
                    .First(c => c != '2'))
                .Select(c => c == '1' ? '#' : ' '))
            .Select(rowChars => new string(rowChars.ToArray()))
            .ToArray();

        Console.WriteLine($"Part 2:");

        foreach (var line in image)
            Console.WriteLine(line);
    }
}
