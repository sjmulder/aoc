namespace day01;

public static class Util
{
    public static IEnumerable<T[]> GroupBySeparator<T>(
        this IEnumerable<T> xs,
        Func<T, bool> separatorPredicate)
    {
        var group = new List<T>();

        foreach (var x in xs)
        {
            if (separatorPredicate(x))
            {
                yield return group.ToArray();
                group.Clear();
            }
            else
            {
                group.Add(x);
            }
        }
    }
}