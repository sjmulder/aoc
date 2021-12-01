using System;
using System.IO;
using System.Linq;

var nums = File.ReadAllLines("../../../../../data/01-input.txt")
    .Select(int.Parse).ToList();
var pairs = nums
    .Zip(nums.Skip(1), (a, b) => a + b)
    .Zip(nums.Skip(2), (a, b) => a + b).ToList();

var inc1 = nums.Zip(nums.Skip(1), (a, b) => a < b).Count(x => x);
var inc3 = pairs.Zip(pairs.Skip(1), (a, b) => a < b).Count(x => x);

Console.WriteLine($"{inc1} {inc3}");