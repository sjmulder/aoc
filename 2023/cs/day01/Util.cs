namespace day01;

public static class Util
{
    public static (T, T) FirstAndLast<T>(this IEnumerable<T> xs)
        where T : struct
    {
        var (first, last) = xs.Aggregate<T, (T?, T?)>(
            (null, null),
            (acc, x) => (acc.Item1 ?? x, x));

        return (first ?? default, last ?? default);
    }

    public static IEnumerable<T> NotNull<T>(this IEnumerable<T?> xs)
            where T : struct
        => xs.Where(x => x != null).Cast<T>();

    public static (int, int) Sum(this IEnumerable<(int, int)> xs)
        => xs.Aggregate(
            (0, 0),
            (a, x) => (a.Item1 + x.Item1, a.Item2 + x.Item2));
}
