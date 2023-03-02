double loop(double x, unsigned n)
{
    double element, sum;
    sum = element = x;
    for (int i = 1; i < n; ++i)
    {
        element = element *
                (x * x) *
                (2 * i - 1) * (2 * i - 1) /
                (4 * (i * i) + 2 * i);
        sum += element;
    }
    return sum;
}
