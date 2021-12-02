string[]? parts;
int h = 0, d = 0, a = 0;

using (var reader = new StreamReader("../../../../../data/02-input.txt"))
    while ((parts = reader.ReadLine()?.Split(' ')) != null)
        switch (parts[0], int.Parse(parts[1]))
        {
            case ("down", var x): a += x; break;
            case ("up", var x): a -= x; break;
            case ("forward", var x): h += x; d += a * x; break;
            default: throw new ApplicationException($"Bad input: {parts[0]}");
        }

Console.WriteLine($"{h * a} {h * d}");