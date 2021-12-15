static class Util
{
    public static long Spread(this IEnumerable<long> nums)
        => nums.Max() - nums.Min();
}
