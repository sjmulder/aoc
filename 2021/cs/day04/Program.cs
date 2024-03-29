﻿// Boards are represented as int[][] - an array of rows.
//
// Bingo checking doesn't work by striking off numbers. Instead rows and columns
// (transposed rows) are mapped into the last turn (draw index) any of their
// numbers are drawn, i.e. when they are completed.
//
// Then the boards are sorted by their first completed row or column to
// determine the completion order.
//
// Finally the scores are determined by filtering out the numbers drawn before
// their first row or column was completed (as per above), leaving the unstruck
// numbers to be summed and multiplied by the last drawn number at that point.

var lines = File.ReadAllLines("../../data/04-input.txt");
var draws = lines.First().Split(',').Select(int.Parse).ToArray();
var winners = lines.Skip(1).Where(l => l != string.Empty)
    .Select(l => l.Split(' ', StringSplitOptions.RemoveEmptyEntries)
        .Select(int.Parse).ToArray()).Chunk(5)
    .Select(board => (board, drawIdx: board
        .Concat(Enumerable.Range(0, 5).Select(c => board.Select(r => r[c])))
        .Select(r => r.Select(x => Array.IndexOf(draws, x)).Max()).Min()))
    .OrderBy(t => t.drawIdx).ToArray();
var scores = new[] { winners.First(), winners.Last() }
    .Select(t => t.board.SelectMany(r => r)
        .Where(x => !draws.Take(t.drawIdx + 1).Contains(x))
        .Sum() * draws[t.drawIdx])
    .ToArray();

Console.WriteLine($"04: {scores[0]} {scores[1]}");
