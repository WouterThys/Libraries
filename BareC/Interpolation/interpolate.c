#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <string.h>



static void findLookupPoint(uint16_t * searchArray, uint16_t length, uint16_t value, uint16_t * x0, uint16_t *y0, uint16_t *x1, uint16_t *y1);
static float interpolate(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x);

static uint16_t array[10][2] = {{1,400},{2, 800},{3,1200}, {4,1600}, {5,2000}, {6,2400}, {7,2800}, {8,3200}, {9,3600}, {10,4000}};


void findLookupPoint(uint16_t * searchArray, uint16_t length, uint16_t value, uint16_t * x0, uint16_t *y0, uint16_t *x1, uint16_t *y1) {
    
    *x0 = *x1 = *y0 = *y1 = -1;
    
    if (value < (searchArray[1])) {
        
        *x0 = 0;
        *x1 = 0;
        
        *y0 = searchArray[*x0 + 1];
        *y1 = searchArray[*x1 + 1];
        
        return;
    }
    
    if (value > searchArray[length]) {

        *x0 = length-1;
        *x1 = length-1;

        *y0 = searchArray[*x0 + 1];
        *y1 = searchArray[*x1 + 1];

        return;
    }
        
    uint16_t i;
    for (i = 0; i < length-1; i++) {

        if (value >= searchArray[i+1] && value < searchArray[i+2]) {

            *x0 = i;
            *x1 = i+1;

            *y0 = searchArray[*x0 + 1];
            *y1 = searchArray[*x1 + 1];

            break;
        }
    }
        
}

float interpolate(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x) {
    float t;

    if (x <= x0) { return y0; }
    if (x >= x1) { return y1; }

    t =  (float)(x-x0);
    t /= (float)(x1-x0);
	
	printf("  D: t=%.2f\n", t);

    return (float)y0 + t*(float)(y1-y0);
}



int main() {
	printf("Start\n");
	
	uint16_t p = 0;
	uint16_t * arrayPtr;
	arrayPtr = &array[0][0];
	
	while (1) {
		
		printf("Enter a point between 0 and 100: ");
		scanf("%d", &p);
		
		printf("Find point %d\n", p);
		
		uint16_t x0, y0, x1, y1;
		
		findLookupPoint(&array[0][0], 10, p, &x0, &y0, &x1, &y1);
		printf("Point %d is between: (%d,%d) and (%d,%d)\n", p, x0, y0, x1, y1);
		
		
		float interp = interpolate(x0, y0, x1, y1, p);
		printf("Interpolated: %.2f\n\n", interp);
	}
}

