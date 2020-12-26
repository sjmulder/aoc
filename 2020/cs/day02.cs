using System;
using System.Linq;
using System.Text.RegularExpressions;

class Program {
	static void Main() {
		int part1=0, part2=0;
		var rx = new Regex(@"^(\d+)-(\d+) (\w): (\w+)$", RegexOptions.Compiled);
		string line;

		while ((line = Console.ReadLine()) != null) {
			var groups = rx.Matches(line)[0].Groups;
			int lo = int.Parse(groups[1].Value);
			int hi = int.Parse(groups[2].Value);
			char ch = groups[3].Value[0];
			string pass = groups[4].Value;

			int count = pass.Count(c => c == ch);
			if (count >= lo && count <= hi)
				part1++;
			if (pass[lo-1] == ch ^ pass[hi-1] == ch)
				part2++;
		}

		Console.WriteLine($"{part1} {part2}");
	}
}
