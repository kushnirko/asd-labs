/* Типи лінії таблиці */
#define FIRST_LINE      1
#define MIDDLE_LINE     2
#define LAST_LINE       3
/* ASCII символи для зображення кожного типу лінії таблиці */
const int first_line_chars[]    = {218, 196, 194, 191};
const int middle_line_chars[]   = {195, 196, 197, 180};
const int last_line_chars[]     = {192, 196, 193, 217};

void PrintTableLine(int cols_quantity, const int cols_lengths[cols_quantity], int line_type)
{
    const int *pointer;
    switch (line_type)
    {
    case FIRST_LINE :
        pointer = first_line_chars;
        break;
    case MIDDLE_LINE :
        pointer = middle_line_chars;
        break;
    case LAST_LINE :
        pointer = last_line_chars;
    }
    int symbols[4];
    int i, j;
    for (i = 0; i < 4; i++)
        symbols[i] = pointer[i];
    printf("%c", symbols[0]);
    for (i = 0; i < cols_quantity; i++)
    {
        for (j = 0; j < cols_lengths[i]; j++)
            printf("%c", symbols[1]);
        if ((i + 1) != cols_quantity)
            printf("%c", symbols[2]);
    }
    printf("%c\n", symbols[3]);
}
