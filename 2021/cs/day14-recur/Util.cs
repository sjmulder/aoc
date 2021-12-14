static class Util
{
    public static IEnumerable<T> Intersperse<T>(
        this IEnumerable<T> elems, Func<T, T, T> selector)
    {
        using var iter = elems.GetEnumerator();

        if (!iter.MoveNext())
            yield break;

        T prev = iter.Current;
        yield return iter.Current;

        while (iter.MoveNext())
        {
            yield return selector(prev, iter.Current);
            yield return iter.Current;
            prev = iter.Current;
        }
    }

    public static Dictionary<T, long> ToFrequencyTable<T>(
        this IEnumerable<T> elems)
    {
        var table = new Dictionary<T, long>();

        foreach (var x in elems)
            table[x] = table.GetValueOrDefault(x, 0) + 1;

        return table;
    }

    public static long Spread(this IEnumerable<long> nums)
        => nums.Max() - nums.Min();
}
