// Brute force 'solution' that runs in O(nsteps) space - good - but O(2^nsteps)
// time - very bad. But it's elegant and that's what really matters!

var lines = File.ReadAllLines("../../data/14-input.txt");

var rules = lines.Skip(2)
    .Select(l => l.Split(" -> "))
    .ToDictionary(a => (a[0][0], a[0][1]), a => a[1][0]);

IEnumerable<char> Step(IEnumerable<char> chars)
    => chars.Intersperse((left, right) => rules[(left, right)]);

IEnumerable<char> Steps(IEnumerable<char> chars, int count)
    => Enumerable.Range(0, count).Aggregate(chars, (acc, _) => Step(acc));

long Score(IEnumerable<char> chars)
    => chars.ToFrequencyTable().Values.Spread();

Console.WriteLine("14 (p1): " + Score(Steps(lines[0], 10)));
Console.WriteLine("14 (p2): " + Score(Steps(lines[0], 40)));
