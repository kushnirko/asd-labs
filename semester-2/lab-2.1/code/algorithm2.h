typedef struct
{
    double element, sum;
} structure;

structure alg2(double x, unsigned int n)
{
    structure result;
    if (n == 1)
        result.sum = result.element = x;
    else
    {
        result = alg2(x, (n - 1));
        --n;
        result.element = result.element *
                (x * x) *
                ((2 * n - 1) * (2 * n - 1)) /
                (4 * (n * n) + 2 * n);
        result.sum += result.element;
    }
    return result;
}
