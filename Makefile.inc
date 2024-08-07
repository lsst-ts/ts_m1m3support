# define to 1 to produce verbose output (on command line, make VERBOSE=1)
ifndef VERBOSE
  co := @
  silence := --silence-errors
endif

# change as needed
ifeq ($(WITH_SAL_KAFKA),1)
  c_opts := -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE -DWITH_SAL_KAFKA
else
  c_opts := -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE
endif

ifdef DEBUG
  c_opts += -g
  silence := --silence-errors
else
  c_opts += -O3
endif

CRIOCPP := ../ts_cRIOcpp/

PKG_CPPFLAGS := \
	$(shell pkg-config yaml-cpp spdlog fmt --cflags $(silence))

PKG_LIBS := \
	$(shell pkg-config yaml-cpp spdlog fmt --libs $(silence)) -lstdc++


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

ifdef WITH_SAL_KAFKA
  PKG_CPPFLAGS += \
	$(shell pkg-config rdkafka rdkafka++ openssl snappy jansson --cflags)
  PKG_LIBS += \
	$(shell pkg-config rdkafka rdkafka++ openssl snappy jansson --libs)

  SAL_CPPFLAGS += $(PKG_CPPFLAGS)  \
        -I${SAL_HOME}/include -I${SAL_HOME}/include/avro
  LIBS += $(PKG_LIBS) -ldl -lpthread -L/usr/lib64/boost${BOOST_RELEASE} -lboost_filesystem -lboost_iostreams \
	-lboost_program_options -lboost_system \
	-lcurl -lavrocpp -lavro
  SAL_LIBS := -lserdes++ -lserdes \
        -lSAL_MTM1M3 -lSAL_MTMount
  LIBS_FLAGS += $(LIBS) -L${SAL_HOME}/lib
else
  SAL_CPPFLAGS += $(PKG_CPPFLAGS) \
	-I${OSPL_HOME}/include -I${OSPL_HOME}/include/sys -I${OSPL_HOME}/include/dcps/C++/SACPP \
	-I${SAL_WORK_DIR}/MTM1M3/cpp/src -I${SAL_WORK_DIR}/MTMount/cpp/src -I${SAL_WORK_DIR}/include \
	-I${SAL_HOME}/include -I${LSST_SDK_INSTALL}/include
  LIBS += $(PKG_LIBS) -ldl
  SAL_LIBS := -ldcpssacpp -ldcpsgapi -lddsuser -lddskernel \
	-lpthread -lddsserialization -lddsconfparser -lddsconf -lddsdatabase \
	-lddsutil -lddsos \
	${SAL_WORK_DIR}/lib/libSAL_MTM1M3.a ${SAL_WORK_DIR}/lib/libSAL_MTMount.a
  LIBS_FLAGS += -L${OSPL_HOME}/lib -L${LSST_SDK_INSTALL}/lib
endif

VERSION := $(shell git describe --tags --dirty 2>/dev/null || echo "unknown:non-git")
GIT_HASH := $(shell git rev-parse HEAD 2>/dev/null || echo "unknown-non-git")
