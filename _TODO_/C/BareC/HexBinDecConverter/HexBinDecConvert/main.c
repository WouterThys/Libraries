#include <stdio.h>
#include <stdlib.h>

// Declarations
typedef enum inputMode {
    HB, HD, BH, BD, DH, DB
} InputMode;

// Function declarations
void selectInputMode(InputMode * inputMode);
void printInputMode(InputMode inputMode);

// Variables
InputMode inputMode = HB;

/*
 * 
 */
int main(int argc, char** argv) {
    
    printf("Convert binary, hexadecimal, decimal input to any other output format.\n");
    printf("To select the input mode press M. Current mode is: \n\t");
    printInputMode(inputMode);
   

    return (EXIT_SUCCESS);
}

void printInputMode(InputMode inputMode) {
    switch(inputMode) {
        case HB: printf("HB: Hexadecimal input to binary output\n"); break;
        case HD: printf("HD: Hexadecimal input to decimal output\n"); break;
        case BH: printf("BH: Binary input to hexadecimal output\n"); break;
        case BD: printf("BD: Binary input to decimal output\n"); break;
        case DH: printf("DH: Decimal input to hexadecimal output\n"); break;
        case DB: printf("DB: Decimal input to binary output\n\n"); break;
        default: break;
    }
}

void selectInputMode(InputMode * inputMode) {
    printf("Select the input output mode: \n");
    printf("\t - "); printInputMode(HB);
    printf("\t - "); printInputMode(HD);
    printf("\t - "); printInputMode(BH);
    printf("\t - "); printInputMode(BD);
    printf("\t - "); printInputMode(DH);
    printf("\t - "); printInputMode(DB);
}

