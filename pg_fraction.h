/*
 * pg_fraction.h
 *
 * define fraction type 
 *
 */

/*
 * Internal representation of fraction type
 */
typedef struct st_fraction {
    int numerator;
    int denominator;
    double value;
} fraction;

