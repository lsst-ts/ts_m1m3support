# define to "" to produce verbose output
#define VERBOSE ""

ifndef VERBOSE
  co := @
  silence := --silence-errors
endif

# change as needed
c_opts := -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE 

ifdef DEBUG
  c_opts += -g
  silence := --silence-errors
else
  c_opts += -O3
endif

CRIOCPP := ../ts_cRIOcpp/

# avro-cpp headers/libs come from the SAL prefix (no avro-cpp.pc there),
# via -I${LSST_SAL_PREFIX}/include and -lavrocpp below.
PKG_CPPFLAGS := $(shell pkg-config yaml-cpp spdlog fmt --cflags $(silence))

# libschemaregistry replaces libserdes (OSW-2238). It is a static archive
# whose transitive deps (abseil, cpr, curl, compression) must be linked
# explicitly; --start-group/--end-group lets the linker resolve their
# cross-references regardless of order. ssl/crypto/lz4 come from the
# image's shared libs.
LIBSCHEMAREGISTRY_VCPKG_LIB ?= ${LSST_SAL_PREFIX}/lib
LIBSCHEMAREGISTRY_LDLIBS = -Wl,--start-group ${LSST_SAL_PREFIX}/lib/libschemaregistry.a $(wildcard ${LIBSCHEMAREGISTRY_VCPKG_LIB}/libabsl_*.a) ${LIBSCHEMAREGISTRY_VCPKG_LIB}/libcpr.a ${LIBSCHEMAREGISTRY_VCPKG_LIB}/libcurl.a ${LIBSCHEMAREGISTRY_VCPKG_LIB}/libz.a ${LIBSCHEMAREGISTRY_VCPKG_LIB}/libbz2.a ${LIBSCHEMAREGISTRY_VCPKG_LIB}/liblzma.a ${LIBSCHEMAREGISTRY_VCPKG_LIB}/libzstd.a -Wl,--end-group

SAL_CPPFLAGS += $(PKG_CPPFLAGS)  \
	-I${SAL_WORK_DIR}/MTM1M3/cpp/src -I${SAL_WORK_DIR}/MTMount/cpp/src \
	-I${SAL_WORK_DIR}/include -I${CRIOCPP} -I. \
	-I${SAL_HOME}/include -I${LSST_SDK_INSTALL}/include -I${SAL_HOME}/include -I${LSST_SAL_PREFIX}/include 
LIBS += $(PKG_LIBS) -ldl -lpthread -L/usr/lib64/boost${BOOST_RELEASE} -lboost_filesystem -lboost_iostreams \
	-lboost_program_options -lboost_system -lsnappy \
	${SAL_WORK_DIR}/lib/libSAL_MTMount.a ${SAL_WORK_DIR}/lib/libSAL_MTM1M3.a \
	-L${LSST_SAL_PREFIX}/lib -L${SAL_WORK_DIR}/lib $(LIBSCHEMAREGISTRY_LDLIBS) -lrdkafka++ -lrdkafka -lavrocpp -lsasl2 -lssl -lcrypto -llz4
LIBS_FLAGS += -L${LSST_SAL_PREFIX}/lib

PKG_LIBS := \
        $(shell pkg-config yaml-cpp --libs $(silence))

CPP := g++ -std=c++17 -Wall -fPIE ${c_opts}

# compile x86 simulator or real cRIO stuff
ifdef SIMULATOR
  C := gcc -Wall ${c_opts}
  CPP += -DSIMULATOR
  PKG_LIBS += $(shell pkg-config spdlog --libs $(silence))
else
  C := gcc -Wall -fmessage-length=0 ${c_opts}
  CPP += -fmessage-length=0
  PKG_LIBS += $(shell pkg-config spdlog --libs $(silence))
endif

LIBS += $(PKG_LIBS) -ldl -lpthread -L/usr/lib64/boost${BOOST_RELEASE} -lboost_filesystem -lboost_iostreams \
	-lboost_program_options -lboost_system -lsnappy \
	-L${LSST_SAL_PREFIX}/lib -lrdkafka++ -lrdkafka -lavrocpp

VERSION := $(shell git describe --tags --dirty 2>/dev/null || echo "unknown:non-git")
GIT_HASH := $(shell git rev-parse HEAD 2>/dev/null || echo "unknown-non-git")
