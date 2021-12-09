int FromBase2(IEnumerable<char> s) => Convert.ToInt32(new string(s.ToArray()), 2);
bool MostlyOnes(IList<string> l, int bit) => l.Count(s => s[bit] == '1') >= (l.Count + 1) / 2;

var ls = File.ReadAllLines("../../../../../data/03-input.txt");
var nbits = ls[0].Length;
var bits = Enumerable.Range(0, ls[0].Length);
var gamma = FromBase2(bits.Select(bit => MostlyOnes(ls, bit) ? '1' : '0'));
var epsilon = ~gamma & ((1 << nbits) - 1);

int Part2(bool flip)
{
    var list = ls.ToList();
    for (int bit = 0; bit < nbits && list.Count > 1; bit++)
    {
        char val = MostlyOnes(list, bit) != flip ? '1' : '0';
        list.RemoveAll(x => x[bit] != val);
    }

    return FromBase2(list[0]);
}

var p1 = gamma * epsilon;
var p2 = Part2(false) * Part2(true);
Console.WriteLine($"03: {p1} {p2}");
