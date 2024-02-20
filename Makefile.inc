# define to "" to produce verbose output
#define VERBOSE ""
ifndef VERBOSE
  co := @
  silence := --silence-errors
endif

# change as needed
c_opts := -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE -DWITH_SAL_KAFKA

ifdef DEBUG
  c_opts += -g
  silence := --silence-errors
else
  c_opts += -O3
endif

# compile x86 simulator or real cRIO stuff
ifdef SIMULATOR
  C := gcc -Wall ${c_opts}
  CPP := g++ -std=c++14 -Wall ${c_opts} -DSIMULATOR
else
  C := gcc -Wall -fmessage-length=0 ${c_opts}
  CPP := g++ -std=c++14 -Wall -fmessage-length=0 ${c_opts}
endif

CRIOCPP := ../ts_cRIOcpp/

PKG_CPPFLAGS := $(shell pkg-config spdlog --cflags $(silence))
SAL_CPPFLAGS += $(PKG_CPPFLAGS)  -I${SAL_HOME}/include -I${LSST_SAL_PREFIX}/include -I${LSST_SAL_PREFIX}/include/avro

PKG_LIBS := $(shell pkg-config yaml-cpp --libs $(silence)) $(shell pkg-config spdlog --libs $(silence))
LIBS += $(PKG_LIBS) -ldl -lpthread /opt/lsst/ts_sal/lib/libSAL_MTM1M3.a /opt/lsst/ts_sal/lib/libSAL_MTMount.a -L/usr/lib64/boost${BOOST_RELEASE} -lboost_filesystem -lboost_iostreams -lboost_program_options -lboost_system /opt/lsst/ts_sal/lib/libserdes++.a /opt/lsst/ts_sal/lib/libserdes.a -L${LSST_SAL_PREFIX}/lib -lcurl -lrdkafka++ -lrdkafka -lavrocpp -lavro -ljansson

LIBS_FLAGS += -L"${LSST_SAL_PREFIX}/lib"

VERSION := $(shell git describe --tags --dirty 2>/dev/null || echo "unknown:non-git")
GIT_HASH := $(shell git rev-parse HEAD 2>/dev/null || echo "unknown-non-git")
