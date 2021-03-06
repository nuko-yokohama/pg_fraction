# pg_fraction - 分数型

pg_fractionは「分子の数/分母の数」の形式で記述した文字列を分数として扱い、四則演算と比較演算と一部の集約演算の能力をもつ、ユーザ定義型を提供するPostgreSQL拡張モジュールです。

pg_fractionでは、以下のような演算を可能とします。

```
test=# SELECT '1/2' + '2/3' as result;
 result 
--------
 7/6
(1 row)
```

fraction型として評価されると、約分した結果が格納されます。

```
test=# SELECT '2/6' AS result; 
 result 
--------
 1/3
(1 row)

```

## 重要な注意

この拡張モジュールは、商用利用を想定したものではなく、PostgreSQLのユーザ定義型の実装のサンプルとして開発したものである。



## バージョン

* 0.1 - 開発中



## サポートする書式

### 形式

```
[<負符号>]{<数字>...} <斜線> {<数字>...} ]
```

* <負符号>はハイフン(-)です。

* <数字>は、十進数字(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)です。

  * Note: <数字>は現バージョンでは5桁までの数とします。6桁以上の数字を記述した場合には、形式エラーとなります。将来的には任意の桁数に対応したいと思いますが、あまり当てにしないでください。

* <斜線>はスラッシュ文字(/)です。


### 記述例

```
fraction=# SELECT '3/4'::fraction;
 fraction 
----------
 3/4
(1 row)

fraction=# SELECT '3/6'::fraction;
 fraction 
----------
 1/2
(1 row)

fraction=# SELECT '0/10000'::fraction;
 fraction 
----------
 0/1
(1 row)

fraction=# SELECT '3/100000'::fraction;
ERROR:  pg_fraction:: overflow error (max=99999) "3/100000"
LINE 1: SELECT '3/100000'::fraction;
               ^
fraction=# SELECT 'foo/bar'::fraction;
ERROR:  pg_fraction:invalid input syntax: "foo/bar"
LINE 1: SELECT 'foo/bar'::fraction;
               ^
fraction=# 
```

## サポートする演算機能

pg_fractionは以下の演算をサポートします。

### 値取得関数

| 関数形式                             | 機能   | 記述例               |
| -------------------------------- | ---- | ----------------- |
| doble precision get_value(fraction) | 分数の値をdouble precisionで返却する   | get_value('1/3') |

### 四則演算子

いずれの関数も、分子または分母数が99999を超えた場合にはエラーとなる。

| 演算子  | 機能           | 記述例           |
| ---- | ------------ | ------------- |
| +    | 左辺に右辺を加算します  | '1/2' + '3/4' |
| -    | 左辺から右辺を減算します | '1/2' - '3/4' |
| *    | 左辺と右辺を乗算します  | '1/2' * '3/4' |
| /    | 左辺から右辺を除算します | '1/2' / '3/4' |

### 集約演算関数

| 関数形式                   | 機能   | 備考                                       |
| ---------------------- | ---- | ---------------------------------------- |
| fraction max(fraction) | 最大値  |                                          |
| fraction min(fraction) | 最小値  |                                          |

### 比較演算子

| 比較演算子 | 機能              | 記述例            |
| ----- | --------------- | -------------- |
| =     | 左辺と両辺が等しい場合は真   | '1/2' = '3/4'  |
| <>    | 左辺と両辺が等しくない場合は真 | '1/2' <> '3/4' |
| <     | 左辺が右辺より小さい場合は真  | '1/2' < '3/4'  |
| <=    | 左辺が右辺以下の場合は真    | '1/2' <= '3/4' |
| >     | 左辺が右辺より大きい場合は真  | '1/2' > '3/4'  |
| >=    | 左辺が右辺以上の場合は真    | '1/2' >= '3/4' |

### インデックスサポート

pg_fractionではB-treeインデックスをサポートします。

### パラレルクエリ

pg_fractionでは、+, -, *, / の算術演算子および、max/min関数がパラレルクエリに対応しています。

## 型変換サポート

* 5桁以内の整数値(-99999～99999)を分数型にキャスト可能です。
* 分子/分母の演算結果をdouble precision型にキャスト可能です。


## インストール

```
$ git clone https://github.com/nuko-yokohama/pg_fraction
$ cd pg_fraction
$ make USE_PGXS=1
$ make USE_PGXS=1 install
$ make USE_PGXS=1 installcheck
$ psql your_database -c "CREATE EXTENSION pg_fraction"
```

## TODO

* README.mdの英語化
* 現バージョンでは分子、分母ともに桁数の制約がある。将来的には桁数制約を除去すべきだが、対応はかなり面倒だと思われる。
  * 桁数チェックは約分前に実施するが、これは約分後に実施するのが望ましい。
* 現バージョンでは比較演算時に、分子数と分母数を一旦、浮動小数点(double)に型変換し、分子/分母の結果(double)、同士で比較する。

## 検証環境

* OS：CentOS 7
* PostgreSQL 10

## 著者

* ぬこ＠横浜(nuko_yokohama)
    * Twitter [@nuko_yokohama](https://twitter.com/nuko_yokohama)

