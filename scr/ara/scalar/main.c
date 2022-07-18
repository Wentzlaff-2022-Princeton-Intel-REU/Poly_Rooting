/*--------------------------------------------------------------------*/
/* main.c                                                             */
/*--------------------------------------------------------------------*/

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "freePoly.h"
#include "newton.h"
#include "reading.h"

/*--------------------------------------------------------------------*/

int main(int argc, char *argv[]) {
    double crit_conversion = 1e-14;
    
    Polynomial_t poly = reading();
    double* roots = newton(poly, crit_conversion);

    if (roots[0] == DBL_MAX) {
        printf("Your polynomial has no roots.\n");
    }
    else {
        for (int i = 0; i < poly.degree; i++) {
            if (roots[i] == DBL_MAX) {
                break;
            }
            printf("The root approximation is: %.18lg \n", roots[i]);
        }
    }
    freePoly(&poly);
    free(roots);
    
    // for (int i = 0; i <= polyd.degree; i++) {
    //     printf("%lf \n", polyd.coefficients[i]);
    // }

    return 0;
}
