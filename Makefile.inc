# define to "" to produce verbose output
ifndef VERBOSE
  co := @
endif

# compile x86 simulator or real cRIO stuff
ifdef SIMULATOR
  C := gcc -O3 -Wall -g
  CPP := g++ -std=c++11 -pedantic -O3 -Wall -g -DSIMULATOR
else
  C := gcc -Wall -g -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"
  CPP := g++ -std=c++11 -Wall -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -g
  m1m3cli := m1m3cli
endif

BOOST_CPPFLAGS := -I/usr/include
SAL_CPPFLAGS := -I${OSPL_HOME}/include -I${OSPL_HOME}/include/sys -I${OSPL_HOME}/include/dcps/C++/SACPP -I${SAL_WORK_DIR}/MTM1M3/cpp/src -I${SAL_WORK_DIR}/MTMount/cpp/src -I${SAL_WORK_DIR}/include -I${SAL_HOME}/include -I${LSST_SDK_INSTALL}/include

LIBS := -ldl -lSAL_MTMount -lSAL_MTM1M3 -lsacpp_MTMount_types -lsacpp_MTM1M3_types -ldcpssacpp -ldcpsgapi -lddsuser -lddskernel -lpthread -lddsserialization -lddsconfparser -lddsconf -lddsdatabase -lddsutil -lddsos

LIBS_FLAGS := -L"${SAL_WORK_DIR}/lib" -L"${OSPL_HOME}/lib" -L"${LSST_SDK_INSTALL}/lib"
