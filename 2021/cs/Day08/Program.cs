// Approach: instead of implementing a generic solver, sort the input
// words by criteria that leave them in a known unqiue order.
//
// Given input:
//
//   acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab
//
// First, sort/group by length:
//
//   ab|dab|eafb|cdfbe gcdfa fbcad|cefabd cdfgeb cagedb|acedgfb
//
// This leaves the two len=5 and len=6 groups in unknown order.
// Counting the intersection of those words with words[1] and words[2]
// results in unique combinations:
//
//   ab|dab|eafb|cdfbe gcdfa fbcad|cefabd cdfgeb cagedb|acedgfb
//    "dab":       2     2     3  |   3      2      3
//    "eafb":      3     2     3  |   4      3      3
//    sum:         5     4     6  |   7      5      6
//
// Sorting the groups internally by that sum leaves the digits in
// a known, unique order that can be easily mapped.

var digits = File.ReadLines("../../../../../data/08-input.txt")
    .Select(line => line.Split('|')
        .Select(side => side
            .Split(' ', StringSplitOptions.RemoveEmptyEntries)
            .Select(word => new string(word.OrderBy(c => c).ToArray())))
        .ToList())
    .Select(sides =>
    {
        var left = sides[0].OrderBy(word => word.Length).ToArray();
        var map = left
            .OrderBy(word => word.Length)
            .ThenBy(word => left[1..3].Sum(other => other.Intersect(word).Count()))
            .Zip("1742536098")
            .ToDictionary(t => t.First, t => t.Second);

        return new string(sides[1].Select(word => map[word]).ToArray());
    })
    .ToList();

var p1 = digits.Sum(d => d.Count(c => "1478".Contains(c)));
var p2 = digits.Sum(int.Parse);
Console.WriteLine($"08: {p1} {p2}");
