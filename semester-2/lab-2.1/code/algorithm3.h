double alg3(
        double x,
        unsigned n,
        unsigned i,
        double currentElement
        )
{
    double sum;
    if (n == 0)
        sum = 0;
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
        sum = alg3(x, (n - 1), (i + 1), currentElement);
        sum += currentElement;
    }
    return sum;
}

