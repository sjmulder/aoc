namespace day03;

public static class Util
{
    public static IEnumerable<T[]> GroupsOf<T>(
        this IEnumerable<T> xs,
        int size)
    {
        var list = new List<T>(size);

        foreach (var x in xs)
        {
            list.Add(x);

            if (list.Count == size)
            {
                yield return list.ToArray();
                list.Clear();
            }
        }
    }
}
