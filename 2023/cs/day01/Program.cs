using day01;

int GetValue(string line, int part)
{
    var (a, b) = Enumerable.Range(0, line.Length)
        .Select(i => line.Substring(i) switch
        {
            var s when s.StartsWith("0") => (int?)0,
            var s when s.StartsWith("1") => 1,
            var s when s.StartsWith("2") => 2,
            var s when s.StartsWith("3") => 3,
            var s when s.StartsWith("4") => 4,
            var s when s.StartsWith("5") => 5,
            var s when s.StartsWith("6") => 6,
            var s when s.StartsWith("7") => 7,
            var s when s.StartsWith("8") => 8,
            var s when s.StartsWith("9") => 9,
            var s when part == 1 && s.StartsWith("zero") => 0,
            var s when part == 1 && s.StartsWith("one") => 1,
            var s when part == 1 && s.StartsWith("two") => 2,
            var s when part == 1 && s.StartsWith("three") => 3,
            var s when part == 1 && s.StartsWith("four") => 4,
            var s when part == 1 && s.StartsWith("five") => 5,
            var s when part == 1 && s.StartsWith("six") => 6,
            var s when part == 1 && s.StartsWith("seven") => 7,
            var s when part == 1 && s.StartsWith("eight") => 8,
            var s when part == 1 && s.StartsWith("nine") => 9,
            _ => null
        })
        .NotNull()
        .FirstAndLast();

    return a * 10 + b;
}

var (p1, p2) = File.ReadLines("../../../inputs/2023/01-input.txt")
    .Select(line => (GetValue(line, 0), GetValue(line, 1)))
    .Sum();

Console.WriteLine($"{p1} {p2}");
