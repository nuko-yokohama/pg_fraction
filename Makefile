# contrib/ksj/Makefile

MODULE_big = pg_fraction
OBJS = pg_fraction.o calc.o

EXTENSION = pg_fraction
DATA = pg_fraction--1.0.sql 

REGRESS = pg_fraction

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/ksj
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
