using System;
using System.IO;
using System.Linq;

var nums = File.ReadAllLines("../../../../../data/01-input.txt")
    .Select(int.Parse).ToList();

var inc1 = nums.Zip(nums.Skip(1), (a, b) => a < b).Count(x => x);
var inc3 = nums.Zip(nums.Skip(3), (a, b) => a < b).Count(x => x);

Console.WriteLine($"01: {inc1} {inc3}");