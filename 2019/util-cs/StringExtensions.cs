using System.Collections.Generic;

namespace Aoc.Util
{
    public static class StringExtensions
    {
        public static IEnumerable<string> Slice(
            this string s, int length)
        {
            for (int i = 0; i < s.Length; i += length)
                yield return s.Substring(i, length);
        }
    }
}
