using day01;

var top = File.ReadLines("../../data/01-input.txt")
    .GroupBySeparator(string.IsNullOrWhiteSpace)
    .Select(group => group.Select(int.Parse).Sum())
    .OrderByDescending(x => x)
    .Take(3)
    .ToList();
    
Console.WriteLine($"01: {top[0]} {top.Sum()}");