var a = File.ReadAllText("../../../../../data/07-input.txt")
    .Split(",").Select(int.Parse).ToArray();
int min = a.Min(), max = a.Max();
int p1 = Enumerable.Range(min, max-min+1).Select(pos => a
    .Select(x => Math.Abs(pos-x)).Sum()).Min();
int p2 = Enumerable.Range(min, max-min+1).Select(pos => a
    .Select(x => Math.Abs(pos-x))
    .Select(x => x*(x+1)/2).Sum()).Min();
Console.WriteLine($"{p1} {p2}");
