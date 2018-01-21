/*
 * pg_fraction.c
 * fraction type postgresql functions
 *
 */

#include <string.h>

#include "postgres.h"
#include "fmgr.h"
#include "utils/geo_decls.h"

#include "calc.h"

/* Version-1 protocol */
#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

/*
 * Internal representation of fraction type
 */
typedef struct ST_Fraction {
    int numerator;
    int denominator;
    double value;
} Fraction;

/*
 * fraction_in()
 * input fraction type 
 */
PG_FUNCTION_INFO_V1(fraction_in);
Datum
fraction_in(PG_FUNCTION_ARGS)
{
    char       *str = PG_GETARG_CSTRING(0);
    int         n, d;
    Fraction   *result;

    /* TODO: more smart parse */

    if (sscanf(str, "%d/%d", &n, &d) != 2)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("pg_fraction:invalid input syntax: \"%s\"",
                        str)));
    /* range check */
    if (d == 0) {
        ereport(ERROR,
                (errcode(ERRCODE_DIVISION_BY_ZERO),
                 errmsg("pg_fraction:division by zero: \"%s\"",
                        str)));
    }

    result = (Fraction* ) palloc(sizeof(Fraction));
    reduce_fraction(&n, &d);

    if ( abs(n) > 99999 || abs(d) > 99999 ) {
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("pg_fraction:: overflow error (max=99999) \"%s\"",
                        str)));
    }

    result->numerator = n;
    result->denominator = d;
    result->value = (double) n / (double) d;
    PG_RETURN_POINTER(result);
}


/*
 * fraction_out()
 * output fraction type 
 */
PG_FUNCTION_INFO_V1(fraction_out);
Datum
fraction_out(PG_FUNCTION_ARGS)
{
    Fraction   *fraction= (Fraction*) PG_GETARG_POINTER(0);
    char       *result;

    result = psprintf("%d/%d", fraction->numerator, fraction->denominator);
    PG_RETURN_CSTRING(result);
}

/*
 * get_value(Fractoin)
 * get double value
 */
PG_FUNCTION_INFO_V1(get_value);
Datum get_value(PG_FUNCTION_ARGS);

Datum get_value(PG_FUNCTION_ARGS) {
    Fraction *f1;
    f1 = (Fraction*) PG_GETARG_POINTER(0);
    PG_RETURN_FLOAT8(f1->value);
}

/*
 * Four arithmetic functions 
 */

PG_FUNCTION_INFO_V1(fraction_add);
Datum fraction_add(PG_FUNCTION_ARGS);

Datum fraction_add(PG_FUNCTION_ARGS) {
    Fraction *f1;
    Fraction *f2;
    Fraction *result;

    int n, d;

    f1 = (Fraction*) PG_GETARG_POINTER(0);
    f2 = (Fraction*) PG_GETARG_POINTER(1);

    add_fraction_internal(
        f1->numerator,
        f1->denominator,
        f2->numerator,
        f2->denominator,
        &n,
        &d);

    result = (Fraction*) palloc(sizeof(Fraction));
    result->numerator = n;
    result->denominator = d;
    result->value = (double) n / (double) d;

    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(fraction_sub);
Datum fraction_sub(PG_FUNCTION_ARGS);

Datum fraction_sub(PG_FUNCTION_ARGS) {
    Fraction *f1;
    Fraction *f2;
    Fraction *result;

    int n, d;

    f1 = (Fraction*) PG_GETARG_POINTER(0);
    f2 = (Fraction*) PG_GETARG_POINTER(1);

    sub_fraction_internal(
        f1->numerator,
        f1->denominator,
        f2->numerator,
        f2->denominator,
        &n,
        &d);

    result = (Fraction*) palloc(sizeof(Fraction));
    result->numerator = n;
    result->denominator = d;
    result->value = (double) n / (double) d;

    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(fraction_mul);
Datum fraction_mul(PG_FUNCTION_ARGS);

Datum fraction_mul(PG_FUNCTION_ARGS) {
    Fraction *f1;
    Fraction *f2;
    Fraction *result;

    int n, d;

    f1 = (Fraction*) PG_GETARG_POINTER(0);
    f2 = (Fraction*) PG_GETARG_POINTER(1);

    mul_fraction_internal(
        f1->numerator,
        f1->denominator,
        f2->numerator,
        f2->denominator,
        &n,
        &d);

    result = (Fraction*) palloc(sizeof(Fraction));
    result->numerator = n;
    result->denominator = d;
    result->value = (double) n / (double) d;

    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(fraction_div);
Datum fraction_div(PG_FUNCTION_ARGS);

Datum fraction_div(PG_FUNCTION_ARGS) {
    Fraction *f1;
    Fraction *f2;
    Fraction *result;

    int n, d;

    f1 = (Fraction*) PG_GETARG_POINTER(0);
    f2 = (Fraction*) PG_GETARG_POINTER(1);

    div_fraction_internal(
        f1->numerator,
        f1->denominator,
        f2->numerator,
        f2->denominator,
        &n,
        &d);

    result = (Fraction*) palloc(sizeof(Fraction));
    result->numerator = n;
    result->denominator = d;
    result->value = (double) n / (double) d;

    PG_RETURN_POINTER(result);
}

/*
 * aggregate functions.
 */

/*
 * max
 */
PG_FUNCTION_INFO_V1(fraction_max);
Datum fraction_max(PG_FUNCTION_ARGS);

Datum fraction_max(PG_FUNCTION_ARGS) {
    Fraction* f_l = (Fraction*) PG_GETARG_POINTER(0);
    Fraction* f_r = (Fraction*) PG_GETARG_POINTER(1);


    if (f_l->value > f_r->value ) {
        PG_RETURN_POINTER(f_l);
    } else {
        PG_RETURN_POINTER(f_r);
    }
}

/*
 * max
 */
PG_FUNCTION_INFO_V1(fraction_min);
Datum fraction_min(PG_FUNCTION_ARGS);

Datum fraction_min(PG_FUNCTION_ARGS) {
    Fraction* f_l = (Fraction*) PG_GETARG_POINTER(0);
    Fraction* f_r = (Fraction*) PG_GETARG_POINTER(1);


    if (f_l->value < f_r->value ) {
        PG_RETURN_POINTER(f_l);
    } else {
        PG_RETURN_POINTER(f_r);
    }
}

/*
 * comparision functoins.
 */
static int
fraction_cmp_internal(double a, double b)
{
    if (a < b)
        return -1;
    if (a > b)
        return 1;
    return 0;
}

PG_FUNCTION_INFO_V1(fraction_eq);
Datum fraction_eq(PG_FUNCTION_ARGS);

Datum
fraction_eq(PG_FUNCTION_ARGS)
{
    Fraction* f1 = (Fraction*) PG_GETARG_POINTER(0);
    Fraction* f2 = (Fraction*) PG_GETARG_POINTER(1);
    double f1_v = f1->value;
    double f2_v = f2->value;

    PG_RETURN_BOOL(fraction_cmp_internal( f1_v, f2_v) == 0);
}

PG_FUNCTION_INFO_V1(fraction_ne);
Datum fraction_ne(PG_FUNCTION_ARGS);

Datum
fraction_ne(PG_FUNCTION_ARGS)
{
    Fraction* f1 = (Fraction*) PG_GETARG_POINTER(0);
    Fraction* f2 = (Fraction*) PG_GETARG_POINTER(1);
    double f1_v = f1->value;
    double f2_v = f2->value;

    PG_RETURN_BOOL(fraction_cmp_internal( f1_v, f2_v) != 0);
}

PG_FUNCTION_INFO_V1(fraction_lt);
Datum fraction_lt(PG_FUNCTION_ARGS);

Datum
fraction_lt(PG_FUNCTION_ARGS)
{
    Fraction* f1 = (Fraction*) PG_GETARG_POINTER(0);
    Fraction* f2 = (Fraction*) PG_GETARG_POINTER(1);
    double f1_v = f1->value;
    double f2_v = f2->value;

    PG_RETURN_BOOL(fraction_cmp_internal( f1_v, f2_v) < 0);
}

PG_FUNCTION_INFO_V1(fraction_gt);
Datum fraction_gt(PG_FUNCTION_ARGS);

Datum
fraction_gt(PG_FUNCTION_ARGS)
{
    Fraction* f1 = (Fraction*) PG_GETARG_POINTER(0);
    Fraction* f2 = (Fraction*) PG_GETARG_POINTER(1);
    double f1_v = f1->value;
    double f2_v = f2->value;

    PG_RETURN_BOOL(fraction_cmp_internal( f1_v, f2_v) > 0);
}

PG_FUNCTION_INFO_V1(fraction_le);
Datum fraction_le(PG_FUNCTION_ARGS);

Datum
fraction_le(PG_FUNCTION_ARGS)
{
    Fraction* f1 = (Fraction*) PG_GETARG_POINTER(0);
    Fraction* f2 = (Fraction*) PG_GETARG_POINTER(1);
    double f1_v = f1->value;
    double f2_v = f2->value;

    PG_RETURN_BOOL(fraction_cmp_internal( f1_v, f2_v) <= 0);
}

PG_FUNCTION_INFO_V1(fraction_ge);
Datum fraction_ge(PG_FUNCTION_ARGS);

Datum
fraction_ge(PG_FUNCTION_ARGS)
{
    Fraction* f1 = (Fraction*) PG_GETARG_POINTER(0);
    Fraction* f2 = (Fraction*) PG_GETARG_POINTER(1);
    double f1_v = f1->value;
    double f2_v = f2->value;

    PG_RETURN_BOOL(fraction_cmp_internal( f1_v, f2_v) >= 0);
}

PG_FUNCTION_INFO_V1(fraction_cmp);
Datum fraction_eq(PG_FUNCTION_ARGS);

/*
 * btree index support function.
 */
Datum
fraction_cmp(PG_FUNCTION_ARGS)
{
    Fraction* f1 = (Fraction*) PG_GETARG_POINTER(0);
    Fraction* f2 = (Fraction*) PG_GETARG_POINTER(1);
    double f1_v = f1->value;
    double f2_v = f2->value;

    PG_RETURN_INT32(fraction_cmp_internal( f1_v, f2_v));
}

/*
 * cast functions.
 */
PG_FUNCTION_INFO_V1(fraction_to_float8);
Datum fraction_to_float8(PG_FUNCTION_ARGS);

Datum
fraction_to_float8(PG_FUNCTION_ARGS)
{
    Fraction* a = (Fraction*) PG_GETARG_POINTER(0);

    PG_RETURN_FLOAT8( a->value );
}

PG_FUNCTION_INFO_V1(int32_to_fraction);
Datum int32_to_fraction(PG_FUNCTION_ARGS);

Datum
int32_to_fraction(PG_FUNCTION_ARGS)
{
    Fraction* f;
    int32 a =  PG_GETARG_INT32(0);
    f = (Fraction*) palloc(sizeof(Fraction));

    f->numerator = a;
    f->denominator = 1;
    f->value = (float8) a;

    PG_RETURN_POINTER(f);
}

