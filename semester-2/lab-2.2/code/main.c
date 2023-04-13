#include <stdio.h>
#include "WorkingWithLinkedList.h"

int main() {
    printf("List creating:\n");
    linkList *listPointer = createList();

    if (listPointer) {
        printf("\nCreated list:\n");
        printList(listPointer);

        printf("\nSorted list:\n");
        listPointer = sortList(listPointer);

        printList(listPointer);

        printf("\nMemory freeing:\n");
        deleteList(listPointer);
    } else {
        printf("Invalid input!");
    }

    printf("\n* press 'ENTER' to exit the program *\n");
    getchar();
    return 0;
}
