var a = File.ReadAllText("../../../../../data/07-input.txt")
    .Split(",").Select(int.Parse).OrderBy(x => x).ToArray();
int p1 = a.Sum(x => Math.Abs(a[a.Length/2]-x));
int p2 = Enumerable.Range((int)a.Average(), 2)
    .Select(pos => a.Sum(x => { int d = Math.Abs(pos-x); return d*(d+1)/2; }))
    .Min();
Console.WriteLine($"{p1} {p2}");
