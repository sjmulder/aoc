var parts = File.ReadLines("../../data/02-input.txt")
    .Select(x => x.Split(' '))
    .Select(x => (x[0], x[1]) switch
    {
        ("A", "X") => (3+1, 0+3),
        ("A", "Y") => (6+2, 3+1),
        ("A", "Z") => (0+3, 6+2),
        ("B", "X") => (0+1, 0+1),
        ("B", "Y") => (3+2, 3+2),
        ("B", "Z") => (6+3, 6+3),
        ("C", "X") => (6+1, 0+2),
        ("C", "Y") => (0+2, 3+3),
        ("C", "Z") => (3+3, 6+1),
        _ => (0, 0)
    })
    .Aggregate((a, b) => (a.Item1 + b.Item1, a.Item2 + b.Item2));
    
Console.WriteLine($"02: {parts.Item1} {parts.Item2}");
