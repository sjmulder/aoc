using System.Collections.Immutable;

var lines = File.ReadAllLines("../../data/03-input.txt");

var p1 = lines
    .Select(l => l
        .Chunk(l.Length / 2)
        .Select(x => x.ToImmutableHashSet())
        .Aggregate((a, b) => a.Intersect(b))
        .First())
    .Sum(Score);

var p2 = lines
    .Select(x => x.ToImmutableHashSet())
    .Chunk(3)
    .Select(g => g.Aggregate((a, b) => a.Intersect(b)).First())
    .Sum(Score);

Console.WriteLine($"02: {p1} {p2}");

int Score(char c) => c % 32 + (char.IsUpper(c) ? 26 : 0);
