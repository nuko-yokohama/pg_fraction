CREATE EXTENSION IF NOT EXISTS pg_fraction;

CREATE TABLE test (id int, data fraction);
INSERT INTO test VALUES (1, '2/3'),(2, '3/7'),(3,'4/20');
TABLE test;

SELECT id, data, data + '2/3' AS add, data - '2/3' AS sub FROM test;
SELECT id, data, data * '2/3' AS mul, data / '2/3' AS div FROM test;

SELECT max(data), min(data) FROM test;

SELECT id, data FROM test WHERE data = '5/25';
SELECT id, data FROM test WHERE data <> '5/25';
SELECT id, data FROM test WHERE data < '5/25';
SELECT id, data FROM test WHERE data > '5/25';
SELECT id, data FROM test WHERE data <= '5/25';
SELECT id, data FROM test WHERE data >= '5/25';

-- order by
SELECT id, data, get_value(data) FROM test ORDER BY data;

-- cast
SELECT '1/3'::fraction::float8;
SELECT 3::fraction;

