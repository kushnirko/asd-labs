#define FIRST_LINE 1
int charsOfFirstLine[] = {218, 196, 194, 191};

#define MIDDLE_LINE 2
int charsOfMiddleLine[] = {195, 196, 197, 180};

#define LAST_LINE 3
int charsOfLastLine[] = {192, 196, 193, 217};

void printTableLine(int numOfCols, int lenOfCols[numOfCols], int typeOfLine) {
    int *pointer;
    switch (typeOfLine) {
        case FIRST_LINE:
            pointer = charsOfFirstLine;
            break;
        case MIDDLE_LINE:
            pointer = charsOfMiddleLine;
            break;
        case LAST_LINE:
            pointer = charsOfLastLine;
    }
    int symbols[4];
    int i, j;
    for (i = 0; i < 4; ++i) symbols[i] = pointer[i];
    printf("%c", symbols[0]);
    for (i = 0; i < numOfCols; ++i) {
        for (j = 0; j < lenOfCols[i]; ++j) printf("%c", symbols[1]);
        if ((i + 1) != numOfCols) printf("%c", symbols[2]);
    }
    printf("%c\n", symbols[3]);
}
