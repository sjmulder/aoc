using System.Drawing;

var lines = File.ReadAllLines("../../data/13-input.txt");

var points = lines.TakeWhile(l => l.Contains(","))
    .Select(l => l.Split(','))
    .Select(a => new Point(int.Parse(a[0]), int.Parse(a[1])))
    .ToHashSet();

var folds = lines.SkipWhile(l => !l.Contains("fold"))
    .Select(l => l.Split(' ').Last().Split('='))
    .Select(a => (axis: a[0][0], pos: int.Parse(a[1])))
    .ToArray();

HashSet<Point> Fold(HashSet<Point> ps, IEnumerable<(char axis, int pos)> fs) =>
    fs.Aggregate(ps, (acc, f) =>
        acc.Select(p => f.axis == 'x'
            ? new Point(f.pos - Math.Abs(f.pos - p.X), p.Y)
            : new Point(p.X, f.pos - Math.Abs(f.pos - p.Y))).ToHashSet());

var first = Fold(points, folds.Take(1));
var final = Fold(points, folds);

Console.WriteLine($"13 (p1): {first.Count}");

var p2 = Enumerable.Range(0, final.Select(p => p.Y).Max() + 1)
    .Select(y => new string(Enumerable
        .Range(0, final.Select(p => p.X).Max() + 1)
        .Select(x => final.Contains(new Point(x, y)) ? '#' : ' ')
        .ToArray()));

foreach (string line in p2)
    Console.WriteLine($"13 (p2): {line}");