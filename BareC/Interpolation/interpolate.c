#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <string.h>



static void findLookupPoint(uint16_t * searchArray, uint16_t length, uint16_t value, uint16_t * x0, uint16_t *y0, uint16_t *x1, uint16_t *y1);
static void interpolate(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x);


void findLookupPoint(uint16_t * searchArray, uint16_t length, uint16_t value, uint16_t * x0, uint16_t *y0, uint16_t *x1, uint16_t *y1) {
    
    *x0 = *x1 = *y0 = *y1 = -1;
    
    if (value < searchArray[0]) {
        
        *x0 = 0;
        *x1 = 0;
        
        *y0 = searchArray[*x0];
        *y1 = searchArray[*x1];
        
        return;
    }
    
    if (value > searchArray[length-1]) {

        *x0 = length-1;
        *x1 = length-1;

        *y0 = searchArray[*x0];
        *y1 = searchArray[*x1];

        return;
    }
        
    uint16_t i;
    for (i = 0; i < length-1; i++) {

        if (value >= searchArray[i] && value < searchArray[i+1]) {

            *x0 = i;
            *x1 = i+1;

            *y0 = searchArray[*x0];
            *y1 = searchArray[*x1];

            break;
        }
    }
        
}

float interpolate(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x) {
    float t;

    if (x <= x0) { return y0; }
    if (x >= x1) { return y1; }

    t =  (x-x0);
    t /= (x1-x0);

    return y0 + t*(y1-y0);
}



int main() {
	printf("Done");
}

