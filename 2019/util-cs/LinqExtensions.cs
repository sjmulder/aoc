using System;
using System.Collections.Generic;

namespace Aoc.Util
{
    public static class Linq
    {
        public static IEnumerable<T[]> Slice<T>(
            this IEnumerable<T> xs,
            int count)
        {
            var enumerator = xs.GetEnumerator();
            var list = new List<T>(count);

            while (enumerator.MoveNext())
            {
                list.Add(enumerator.Current);
                if (list.Count >= count)
                {
                    yield return list.ToArray();
                    list = new List<T>(count);
                }
            }

            if (list.Count > 0)
                yield return list.ToArray();
        }

        public static T MinBy<T, TKey>(
                this IEnumerable<T> xs,
                Func<T, TKey> selector)
            where TKey : IComparable<TKey>
        {
            var minElem = default(T);
            var minVal = default(TKey);
            bool haveMin = false;

            foreach (var x in xs)
            {
                var val = selector(x);
                if (!haveMin || val.CompareTo(minVal) < 0)
                {
                    minVal = val;
                    minElem = x;
                    haveMin = true;
                }
            }

            return minElem;
        }
    }
}
