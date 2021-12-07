var a = File.ReadAllText("../../../../../data/07-input.txt")
    .Split(",").Select(int.Parse).OrderBy(x => x).ToArray();
int p1 = a.Sum(x => Math.Abs(a[a.Length/2]-x));
int p2 = Enumerable.Range((int)Math.Floor(a.Average()), 2)
    .Select(pos => a
        .Select(x => Math.Abs(pos-x))
        .Select(x => x*(x+1)/2).Sum())
    .Min();
Console.WriteLine($"{p1} {p2}");
