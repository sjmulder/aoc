open System
open System.IO

let nums =
  File.ReadAllLines("../../data/01-input.txt")
  |> Seq.map int

let part1 =
  Seq.zip nums nums
  |> Seq.filter (fun (a, b) -> a + b = 2020)
  |> Seq.map (fun (a, b) -> a * b)
  |> Seq.head

let part2 =
  Seq.zip3 nums nums nums
  |> Seq.filter (fun (a, b, c) -> a + b + c = 2020)
  |> Seq.map (fun (a, b, c) -> a * b * c)
  |> Seq.head

Console.WriteLine part1
Console.WriteLine part2
