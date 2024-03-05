#define VERBOSE ""
WITH_SAL_KAFKA=1

ifndef VERBOSE
  co := @
  silence := --silence-errors
endif

ifdef WITH_SAL_KAFKA
  ls
endif

