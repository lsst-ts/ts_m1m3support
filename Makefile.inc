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

CRIOCPP := ../ts_cRIOcpp/

PKG_CPPFLAGS := \
	$(shell pkg-config yaml-cpp --cflags $(silence)) \
	$(shell pkg-config spdlog --cflags $(silence)) \
	$(shell pkg-config fmt --cflags $(silence))

SAL_CPPFLAGS += $(PKG_CPPFLAGS)  \
	-I${SAL_WORK_DIR}/MTM1M3/cpp/src -I${SAL_WORK_DIR}/MTMount/cpp/src -I${SAL_WORK_DIR}/include \
	-I${SAL_HOME}/include -I${LSST_SDK_INSTALL}/include

PKG_LIBS := \
	$(shell pkg-config yaml-cpp --libs $(silence)) -lstdc++fs

SAL_CPPFLAGS += $(PKG_CPPFLAGS)  -I${SAL_HOME}/include -I${LSST_SAL_PREFIX}/include -I${LSST_SAL_PREFIX}/include/avro

LIBS += $(PKG_LIBS) -ldl -lpthread /opt/lsst/ts_sal/lib/libSAL_MTM1M3.a /opt/lsst/ts_sal/lib/libSAL_MTMount.a -L/usr/lib64/boost${BOOST_RELEASE} -lboost_filesystem -lboost_iostreams -lboost_program_options -lboost_system /opt/lsst/ts_sal/lib/libserdes++.a /opt/lsst/ts_sal/lib/libserdes.a -L${LSST_SAL_PREFIX}/lib -lcurl -lrdkafka++ -lrdkafka -lavrocpp -lavro -ljansson

CPP := g++ -std=c++17 -Wall -fPIE ${c_opts}

# compile x86 simulator or real cRIO stuff
ifdef SIMULATOR
  C := gcc -Wall ${c_opts}
  CPP += -DSIMULATOR
  PKG_LIBS += $(shell pkg-config spdlog --libs $(silence))
else
  C := gcc -Wall -fmessage-length=0 ${c_opts}
  CPP += -fmessage-length=0
  PKG_LIBS += $(shell pkg-config spdlog --libs $(silence) | sed -E 's/-l([a-z0-9]*)/-l:lib\1.a/g')
endif

LIBS += $(PKG_LIBS) -ldl 

SAL_LIBS := ${LSST_SAL_PREFIX}/lib/libSAL_MTM1M3.a ${LSST_SAL_PREFIX}/lib/libSAL_MTMount.a

LIBS_FLAGS += -L${LSST_SAL_PREFIX}/lib

VERSION := $(shell git describe --tags --dirty 2>/dev/null || echo "unknown:non-git")
GIT_HASH := $(shell git rev-parse HEAD 2>/dev/null || echo "unknown-non-git")
