double alg1(
        double x,
        unsigned int n,
        unsigned int i,
        double currentElement,
        double currentSum
        )
{
    double item;
    if (n == 0)
        item = currentSum;
    else
    {
        if (i == 0)
            currentElement = x;
        else
        {
            currentElement = currentElement *
                    (x * x) *
                    ((2 * i - 1) * (2 * i - 1)) /
                    (4 * (i * i) + 2 * i);
        }
        currentSum += currentElement;
        item = alg1(x, (n - 1), (i + 1), currentElement, currentSum);
    }
    return item;
}

