var top = File.ReadAllText("../../data/01-input.txt")
    .Replace("\r\n", "\n")
    .TrimEnd('\n')
    .Split("\n\n")
    .Select(s => s.Split("\n").Select(int.Parse).Sum())
    .OrderByDescending(x => x)
    .Take(3)
    .ToList();
    
Console.WriteLine($"01: {top[0]} {top.Sum()}");