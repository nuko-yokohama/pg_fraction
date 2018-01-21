/*
 * calc.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "calc.h"

/*
 * reduce function.
 * return 1:reduced, 0:not reduced
 * int* numerator(in-out parameter)
 * int* denominator(in-out parameter)
 */
int reduce_fraction(int *n, int *d) {
    int loop_end;
    int r = 0; /* was not reduce */
    int i; /* loop counter */

    if ( *n == *d ) {
        *n = 1;
        *d = 1;
        return 1;
    }

    if ( *n == 0 ) {
        *n = 0;
        *d = 1;
        return 1;
    }

    if ( abs(*n) > abs(*d) ) {
        loop_end = abs(*d);
    } else {
        loop_end = abs(*n);
    }

    i = 2;
    while (i <= loop_end) {
        if ( (*n % i) == 0 && (*d % i) == 0) {
            /* Both the numerator and denominator were divisible */
            *n = *n / i;
            *d = *d / i;
            r = 1; /* was redueced */
        } else {
            i++; 
        }
    }

    /* convert minus sign */
    if ( *n < 0 && *d < 0 ) {
        *n = abs(*n);
        *d = abs(*d);
    }

    return r;
}

/*
 * Calculation Functions
 */

/*
 * Add Functoin
 */
int add_fraction_internal(int n1, int d1, int n2, int d2, int *on, int *od) {
    int t1n, t2n, td;

    t1n = n1 * d2;
    t2n = n2 * d1;
    td = d1 * d2;

    *on = t1n + t2n;
    *od = td;

    reduce_fraction(on, od);
    return 0;
}

/*
 * Sub Functoin
 */
int sub_fraction_internal(int n1, int d1, int n2, int d2, int *on, int *od) {
    int t1n, t2n, td;

    t1n = n1 * d2;
    t2n = n2 * d1;
    td = d1 * d2;

    *on = t1n - t2n;
    *od = td;

    reduce_fraction(on, od);
    return 0;
}

/*
 * Mul Functoin
 */
int mul_fraction_internal(int n1, int d1, int n2, int d2, int *on, int *od) {
    *on = n1 * n2;
    *od = d1 * d2;

    reduce_fraction(on, od);
    return 0;
}

/*
 * Div Functoin
 */
int div_fraction_internal(int n1, int d1, int n2, int d2, int *on, int *od) {
    *on = n1 * d2;
    *od = d1 * n2;

    reduce_fraction(on, od);
    return 0;
}


#ifdef REDUCE
int
main(int argc, char** argv) {
   int r;
   int n;
   int d;

   if (argc != 3) {
     printf("reduce num, demo\n");
     return 0;
   }

   n = atoi(argv[1]);
   d = atoi(argv[2]);
   
   printf("before r=%d, n=%d,d=%d\n", r, n, d);
   r = reduce_fraction( &n, &d );
   printf("after  r=%d, n=%d,d=%d\n", r, n, d);
}
#endif

#ifdef CALC
int
main(int argc, char** argv) {
   int r;
   int n1;
   int d1;
   int n2;
   int d2;
   int on;
   int od;
   double f1, f2;

   if (argc != 5) {
     printf("calc num1 demo1 num2 demo2\n");
     return 0;
   }

   n1 = atoi(argv[1]);
   d1 = atoi(argv[2]);
   n2 = atoi(argv[3]);
   d2 = atoi(argv[4]);
   
   f1 = (double) n1 / (double) d1;
   printf("n1=%d,d1=%d,f1=%f\n", n1, d1, f1);
   f2 = (double) n2 / (double) d2;
   printf("n2=%d,d2=%d,f2=%f\n", n2, d2, f2);

   r = add_fraction_internal( n1, d1, n2, d2, &on, &od );
   printf("add_fraction n1=%d,d1=%d,n2=%d,d2=%d,on=%d,od=%d\n", n1, d1, n2, d2,on,od);
   r = sub_fraction_internal( n1, d1, n2, d2, &on, &od );
   printf("sub_fraction n1=%d,d1=%d,n2=%d,d2=%d,on=%d,od=%d\n", n1, d1, n2, d2,on,od);
   r = mul_fraction_internal( n1, d1, n2, d2, &on, &od );
   printf("mul_fraction n1=%d,d1=%d,n2=%d,d2=%d,on=%d,od=%d\n", n1, d1, n2, d2,on,od);
   r = div_fraction_internal( n1, d1, n2, d2, &on, &od );
   printf("div_fraction n1=%d,d1=%d,n2=%d,d2=%d,on=%d,od=%d\n", n1, d1, n2, d2,on,od);

}
#endif
