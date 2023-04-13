#define NEW_LINE_ASCII 10
#define SPACE_ASCII 32
#define DASH_ASCII 45
const int digitsInASCII[] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57};

#define MAX_INT 2147483647
#define MIN_INT (-2147483648)

int checkDigit(int data) {
    int isDigit = 0, i;
    for (i = 0; i < 10; ++i) {
        if (data == digitsInASCII[i]) isDigit = 1;
    }
    return isDigit;
}

int convertASCIIToInt(int data) {
    return (int)(data - 48);
}

typedef struct validationResult {
    int value;
    int isValid;
} validRes;

validRes askInt() {
    validRes data;
    data.isValid = 1;
    data.value = 0;
    int isNegative = 0;
    long long accumulator = 0;
    int input = getchar();
    while (input == SPACE_ASCII) input = getchar();
    if (input == DASH_ASCII) isNegative = 1;
    else if (checkDigit(input)) accumulator += convertASCIIToInt(input);
    else data.isValid = 0;
    while (input != NEW_LINE_ASCII && data.isValid) {
        input = getchar();
        if (checkDigit(input)) {
            accumulator = (accumulator * 10) + convertASCIIToInt(input);
            if (accumulator < MIN_INT || accumulator > MAX_INT) {
                data.isValid = 0;
            }
        } else if (input != SPACE_ASCII && input != NEW_LINE_ASCII) {
            data.isValid = 0;
        }
    }
    if (data.isValid) {
        data.value = (int) accumulator;
        if (isNegative) data.value *= -1;
    }
    while (input != NEW_LINE_ASCII) input = getchar(); // clear user's input if it is invalid
    return data;
}
