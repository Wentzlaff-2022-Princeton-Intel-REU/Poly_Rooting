/*------------------------------------------------------------*/
/* main.c                                                     */
/* Author: Manya Zhu                                          */
/*------------------------------------------------------------*/

#include <stdlib.h>
#include "reading.h"
#include "newton.h"
#include "derivative.h"

/*------------------------------------------------------------*/

int main(int argc, char *argv[]) {

    if(argc != 2){
        fprintf(stderr, "Usage: %s critical conversion\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    double value;
    Polynomial_t poly = readPoly();
    Polynomial_t polyd = diffrentiatePoly(poly);
    double crit_conversion = strtod(argv[1], NULL);
    value = guess(poly, polyd, crit_conversion);
    printf("The root approximation is: %lf \n", value);

    return 0;
}
