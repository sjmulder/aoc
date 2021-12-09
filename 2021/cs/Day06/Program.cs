var input = File.ReadAllText("../../data/06-input.txt").Split(",").Select(int.Parse).ToArray();
var day0 = Enumerable.Range(0, 9).Select(age => input.LongCount(x => x == age)).ToArray();

long Solve(int days) => Enumerable.Range(0, days)
   .Aggregate(day0, (a, _) => a[1..7].Concat(new[] { a[7] + a[0], a[8], a[0] }).ToArray())
   .Sum();

Console.WriteLine($"06: {Solve(80)}, {Solve(256)}");