/*--------------------------------------------------------------------*/
/* reading.c                                                          */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "reading.h"

/*--------------------------------------------------------------------*/

Polynomial_t reading(void) {
    int n = 0;
    scanf("%d", &n);

    double* a_n = (double*)malloc(sizeof(double) * (n + 1));
    if (a_n == NULL) {
        exit(2);
    }

    for (int i = n; i >= 0; i--) {
        scanf("%lf", &a_n[i]);
    }

    Polynomial_t poly;
    poly.degree = n;
    poly.coefficients = a_n;

    return poly;
}