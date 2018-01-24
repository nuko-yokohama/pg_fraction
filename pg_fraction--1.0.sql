/* pg_fraction--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_fraction" to load this file. \quit

CREATE TYPE fraction;

CREATE FUNCTION fraction_in(cstring)
RETURNS fraction
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION fraction_out(fraction)
RETURNS cstring
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT IMMUTABLE;

CREATE TYPE fraction (
        INTERNALLENGTH = 16,
        INPUT = fraction_in,
        OUTPUT = fraction_out,
        STORAGE = plain
);

--
-- get double precision value
--
CREATE FUNCTION get_value(fraction)
RETURNS double precision
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

--
-- Four arithmetic operations
-- 
CREATE FUNCTION fraction_add(fraction, fraction)
RETURNS fraction
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR + (
    leftarg = fraction,
    rightarg = fraction,
    procedure = fraction_add,
    commutator = +
);

CREATE FUNCTION fraction_sub(fraction, fraction)
RETURNS fraction
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR - (
    leftarg = fraction,
    rightarg = fraction,
    procedure = fraction_sub
);

CREATE FUNCTION fraction_mul(fraction, fraction)
RETURNS fraction
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR * (
    leftarg = fraction,
    rightarg = fraction,
    procedure = fraction_mul
);

CREATE FUNCTION fraction_div(fraction, fraction)
RETURNS fraction
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR / (
    leftarg = fraction,
    rightarg = fraction,
    procedure = fraction_div
);

--
-- Aggregation Functions
--

-- MAX
CREATE FUNCTION fraction_max(fraction, fraction)
RETURNS fraction
AS 'MODULE_PATHNAME'
PARALLEL SAFE
IMMUTABLE
LANGUAGE C STRICT
;

CREATE AGGREGATE max (fraction)
(
sfunc = fraction_max,
stype = fraction,
initcond = '-99999/1',
parallel = safe
);

-- MIN
CREATE FUNCTION fraction_min(fraction, fraction)
RETURNS fraction
AS 'MODULE_PATHNAME'
PARALLEL SAFE
IMMUTABLE
LANGUAGE C STRICT
;

CREATE AGGREGATE min (fraction)
(
sfunc = fraction_min,
stype = fraction,
initcond = '99999/1',
parallel = safe
);

--
-- cast 
--
CREATE FUNCTION fraction_to_float8(fraction)
RETURNS double precision
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (fraction AS double precision) WITH FUNCTION fraction_to_float8(fraction);

CREATE FUNCTION int32_to_fraction(integer)
RETURNS fraction
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (integer AS fraction) WITH FUNCTION int32_to_fraction(integer);

--
-- compare functions.
--

-- eq function and operator
CREATE FUNCTION fraction_eq(fraction, fraction)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
    leftarg = fraction,
    rightarg = fraction,
    procedure = fraction_eq,
    commutator = =,
    RESTRICT = eqsel
);

-- ne function and operator
CREATE FUNCTION fraction_ne(fraction, fraction)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <> (
    leftarg = fraction,
    rightarg = fraction,
    procedure = fraction_ne,
    commutator = =,
    RESTRICT = neqsel
);

-- lt function and operator
CREATE FUNCTION fraction_lt(fraction, fraction)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
    leftarg = fraction,
    rightarg = fraction,
    procedure = fraction_lt,
    RESTRICT = scalarltsel
);

-- gt function and operator
CREATE FUNCTION fraction_gt(fraction, fraction)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR > (
    leftarg = fraction,
    rightarg = fraction,
    procedure = fraction_gt,
    RESTRICT = scalargtsel
);

-- le function and operator
CREATE FUNCTION fraction_le(fraction, fraction)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <= (
    leftarg = fraction,
    rightarg = fraction,
    procedure = fraction_le,
    RESTRICT = scalarltsel
);

-- ge function and operator
CREATE FUNCTION fraction_ge(fraction, fraction)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR >= (
    leftarg = fraction,
    rightarg = fraction,
    procedure = fraction_ge,
    RESTRICT = scalargtsel
);

--
-- B-tree index support 
--
CREATE FUNCTION fraction_cmp(fraction, fraction)
RETURNS integer
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

-- freaction operator class
CREATE OPERATOR CLASS fraction_ops
DEFAULT FOR TYPE fraction USING btree AS
OPERATOR        1       < ,
OPERATOR        2       <= ,
OPERATOR        3       = ,
OPERATOR        4       >= ,
OPERATOR        5       > ,
FUNCTION        1       fraction_cmp(fraction, fraction);
