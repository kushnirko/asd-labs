#include <stdlib.h>
#include "InputValidation.h"
#include "PrimitiveTableOutput.h"

// the letter P in a variable or parameter name means "pointer"

typedef struct linkedList {
    int key;
    struct linkedList *next;
} linkList;

linkList *initList(int data) {
    linkList *firstP;
    firstP = malloc(sizeof(linkList));
    firstP->key = data;
    firstP->next = NULL;
    return firstP;
}

linkList *addItem(linkList *listP, int data) {
    linkList *newP;
    newP = malloc(sizeof(linkList));
    newP->key = data;
    newP->next = listP;
    return newP;
}

linkList *createManually(int numOfItems) {
    printf("Enter a value of key for item 1\n> ");
    validRes data = askInt();
    if (!data.isValid) return NULL;
    linkList *currentP = initList(data.value);
    int counter = 2; // counter shows current item while user enters values
    for (int i = 1; i < numOfItems; ++i) {
        printf("Enter a value of key for item %d\n> ", counter);
        data = askInt();
        while (!data.isValid) {                             //If creation of the list is started,
            printf("Invalid input! Try again!\n> ");        //it cannot be exited, otherwise the process
            data = askInt();                               //of freeing memory won't take place
        }
        ++counter;
        currentP = addItem(currentP, data.value);
    }
    return currentP;
}

linkList *createRandomly(int numOfItems) {
    int data = (rand() % 201);
    if (data > 100) data -= 201; // -100 <= data <= 100
    linkList *currentP = initList(data);
    for (int i = 1; i < numOfItems ; ++i) {
        data = (rand() % 201);
        if (data > 100) data -= 201;
        currentP = addItem(currentP, data);
    }
    return currentP;
}

linkList *createList() {
    printf("How many items should be created?\n> ");
    validRes numOfItems = askInt();
    if (!(numOfItems.isValid && numOfItems.value > 0)) {
        return NULL;
    }
    printf("Do you want to manually enter the values of\n"
           "keys for items or set pseudo-random keys?\n"
           "* 1st variant - enter 1 and press 'ENTER' *\n"
           "* 2nd variant - enter 2 and press 'ENTER' *\n> ");
    validRes input = askInt();
    linkList *listP;
    switch (input.value) {
        case 1:
            listP = createManually(numOfItems.value);
            break;
        case 2:
            listP = createRandomly(numOfItems.value);
            break;
        default:
            listP = NULL;
    }
    system("cls");
    return listP;
}

linkList *sortList(linkList *listP) {
    linkList *previousP = listP;
    linkList *currentP = previousP->next;
    while (listP->key <= 0 && currentP) {
        if (currentP->key >= 0) {
            previousP->next = currentP->next;
            currentP->next = listP;
            listP = currentP;
        } else previousP = currentP;
        currentP = previousP->next;
    }
    linkList *middleP = listP;
    while (currentP) {
        if (currentP->key > 0) {
            previousP->next = currentP->next;
            currentP->next = listP;
            listP = currentP;
        } else if (currentP->key == 0 && previousP->key < 0) {
            previousP->next = currentP->next;
            currentP->next = middleP->next;
            middleP->next = currentP;
        } else previousP = currentP;
        currentP = previousP->next;
    }
    return listP;
}

void printList(linkList *listP) {
    int numOfCols = 4;
    int lenOfCols[] = {5, 18, 13, 18};
    linkList *currentP = listP;
    int counter = 1;
    printTableLine(numOfCols, lenOfCols, 1);
    printf("%c No. %c   item pointer   %c     key     %c   next pointer   %c\n",
           179, 179, 179, 179, 179);
    while (currentP) {
        printTableLine(numOfCols, lenOfCols, 2);
        printf("%c %2d  %c %p %c %11d %c %p %c\n",
               179, counter, 179, currentP, 179, currentP->key, 179, currentP->next, 179);
        currentP = currentP->next;
        ++counter;
    }
    printTableLine(numOfCols, lenOfCols, 3);
}

void deleteList(linkList *listP) {
    int numOfCols = 3;
    int lenOfCols[] = {26, 13, 18};
    linkList *currentP = listP;
    printTableLine(numOfCols, lenOfCols, 1);
    printf("%c   current list pointer   %c     key     %c   next pointer   %c\n",
           179, 179, 179, 179);
    while (currentP) {
        printTableLine(numOfCols, lenOfCols, 2);
        printf("%c     %p     %c %11d %c %p %c\n",
               179, currentP, 179, currentP->key, 179, currentP->next, 179);
        linkList *temporaryP = currentP;
        currentP = currentP->next;
        free(temporaryP);
    }
    printTableLine(numOfCols, lenOfCols, 2);
    printf("%c     %p     %c             %c                  %c\n",
           179, currentP, 179, 179, 179);
    printTableLine(numOfCols, lenOfCols, 3);
    listP = NULL;
}
