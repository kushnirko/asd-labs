#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "algorithm1.h"
#include "algorithm2.h"
#include "algorithm3.h"
#include "loop.h"

double askX()
{
    double value;
    printf("  Enter your x in range (-1; 1): ");
    scanf("%lf", &value);
    return value;
}

unsigned int askN()
{
    unsigned int value;
    printf("  Enter your n (1, 2, 3...): ");
    scanf("%d", &value);
    return value;
}

void printMenu()
{
    printf("Hi! What do you want to do?\n"
            "1. Run algorithm 1\n"
            "2. Run algorithm 2\n"
            "3. Run algorithm 3\n"
            "4. Perform calculations using a loop\n"
            "5. Get the value using asin() function\n"
            "6. Clear the console\n"
            "7. Exit\n"
            "*to select an option enter its number and press 'Enter';\n\n");
}

int main()
{
    printMenu();
    unsigned int isOpen = 1;

    while (isOpen)
    {
        printf("> ");
        char choice = '_';
        char input[2];
        scanf("%c", &input[0]);
        if (input[0] != '\n')
        {
            scanf("%c", &input[1]);
            if (input[1] == '\n')
                choice = input[0];
        }

        switch (choice)
        {
            case '1':
                printf("  Approximation result: %.12lf\n\n",
                       alg1(askX(), askN(), 0, 0, 0));
                break;
            case '2':
                printf("  Approximation result: %.12lf\n\n",
                       alg2(askX(), askN()).sum);
                break;
            case '3':
                printf("  Approximation result: %.12lf\n\n",
                       alg3(askX(), askN(), 0, 0));
                break;
            case '4':
                printf("  Approximation result: %.12lf\n\n",
                       loop(askX(), askN()));
                break;
            case '5':
                printf("  Calculation result: %.12lf\n\n",
                       asin(askX()));
                break;
            case '6':
                system("cls");
                printMenu();
                input[0] = '\n';
                break;
            case '7':
                isOpen = 0;
                input[0] = '\n';
                break;
            default:
                printf("  Whoops... A wrong input! Try again\n\n");
                if (input[1] == '\n')
                    input[0] = '\n';
        }
        while (input[0] != '\n')
            scanf("%c", &input[0]);
    }
    return 0;
}
