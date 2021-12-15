var lines = File.ReadAllLines("../../data/14-input.txt");

var rules = lines.Skip(2)
    .Select(l => l.Split(" -> "))
    .ToDictionary(a => (a[0][0], a[0][1]), a => a[1][0]);

var charCounts = lines[0]
    .GroupBy(x => x).Select(gp => (ch: gp.Key, count: gp.LongCount()))
    .ToArray();

var pairCounts = lines[0].Zip(lines[0].Skip(1), (a, b) => (a, b))
    .GroupBy(x => x).Select(gp => (pair: gp.Key, count: gp.LongCount()))
    .ToArray();

long p1 = 0;

for (int step = 0; step < 40; step++)
{
    charCounts = charCounts
        .Concat(pairCounts.Select(pc => (ch: rules[pc.pair], pc.count)))
        .GroupBy(cc => cc.ch)
        .Select(gp => (ch: gp.Key, count: gp.Sum(cc => cc.count)))
        .ToArray();

    pairCounts = pairCounts
        .SelectMany(pc => new[]
        {
            (pair: (pc.pair.a, rules[pc.pair]), count: pc.count),
            (pair: (rules[pc.pair], pc.pair.b), count: pc.count)
        })
        .GroupBy(pc => pc.pair)
        .Select(gp => (pc: gp.Key, count: gp.Sum(pc => pc.count)))
        .ToArray();

    if (step == 9)
        p1 = charCounts.Select(cc => cc.count).Spread();
}

long p2 = charCounts.Select(cc => cc.count).Spread();

Console.WriteLine($"14: {p1} {p2}");