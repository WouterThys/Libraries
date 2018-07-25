#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define M 10

double simpleLpf(double *x, double *y, double a, int L, double xm);

void main() {

    double x[M] = {5,5,5,5,5,1,1,1,1,1};
    double y[M];

    int N = M/2;
    double mem = 0;
    mem = simpleLpf(x, y, 0.5, N, mem);
    mem = simpleLpf(&x[N], &y[N], 0.5, N, mem);

    // Print
    int i = 0;
    for (i=0; i<M; i++) {
        printf("%f,%f\n", x[i], y[i]);
    }

    exit(0);
}

/*
* y(n) = x(n) + a * x(n-1)
*/
double simpleLpf(double *x, double *y, double a, int L, double xm) {
    int n;
    y[0] = x[0] + a * xm;
    for (n=1; n < L; n++) {
        y[n] = x[n] + a * x[n-1];
    }
    return x[L-1];
}
