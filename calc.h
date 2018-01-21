/*
 * calc.h
 * define calc-utility functions.
 *
 */

extern int reduce_fraction(int *n, int *d);
extern int add_fraction_internal(int n1, int d1, int n2, int d2, int *on, int *od);
extern int sub_fraction_internal(int n1, int d1, int n2, int d2, int *on, int *od);
extern int mul_fraction_internal(int n1, int d1, int n2, int d2, int *on, int *od);
extern int div_fraction_internal(int n1, int d1, int n2, int d2, int *on, int *od);

