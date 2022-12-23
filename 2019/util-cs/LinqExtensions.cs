using System;
using System.Collections.Generic;
using System.Linq;

namespace Aoc.Util
{
    public static class Linq
    {
        public static IEnumerable<T> Flatten<T>(
            this IEnumerable<IEnumerable<T>> xxs)
        {
            return xxs.SelectMany(xs => xs);
        }

        public static int Count<T>(this IEnumerable<T> xs, int val)
        {
            return xs.Aggregate(0, (acc, x) => acc += val.Equals(x) ? 1 : 0);
        }

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
    }
}
