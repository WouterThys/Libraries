#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fifo.h"

#define BUFFER_MAX  5


// Variables
char queue[BUFFER_MAX];
int rear = -1;


void main() {

    int choice = 0;

    while(1) {
        printf("\n\n");
        printf("1 - Insert element \n");
        printf("2 - Take element \n");
        printf("3 - Display all \n");
        printf("4 - Quit \n\n");

        printf("Input: ");
        scanf("%d", &choice);

        char value;
        switch(choice) {
            case 1:
                printf("\t Enter value to insert: ");
                scanf(" %c", &value);
                insert(value);
                break;
            case 2:
                if (take(&value)) {
                    printf("\t -> Took %c\n", value);
                }
                break;
            case 3:
                display();
                break;
            case 4:
                exit(1);
                break;
            default:
                printf("\t Invalid choice..\n");        

        }
    }
}

bool insert(char value) {
    if (rear >= BUFFER_MAX) {
        printf("\t Buffer Overflow\n");
        return false;    
    } 
    rear++;
    queue[rear] = value;
    return true;
}

bool take(char * value) {
    if (rear < 0) {
        printf("\t Queue Underflow\n");   
        return false; 
    } 
    *value = queue[0]; // Always take first element
    
    // Shift values
    int i = 0;
    for (i = 1; i <= rear; i++) {
        queue[i-1] = queue[i];
    } 
    rear--;

    return true;
}

void display() {
    printf("\t Rear = %d\n", rear);
    printf("\t Values: \n");
    
    if (rear >= 0) {
        int i;
        int j = 0;
        for(i = 0; i <= rear; i++) {
            printf("\t - %d: %c\n", j, queue[i]);
            j++;
        } 
    } 
}




























