include ../Makefile.inc

all: compile

.PHONY: FORCE compile run junit clean

TEST_SRCS := $(shell ls test_*.cpp 2>/dev/null)
BINARIES := $(patsubst %.cpp,%,$(TEST_SRCS))
DEPS := $(patsubst %.cpp,%.cpp.d,$(TEST_SRCS))
JUNIT_FILES := $(shell ls *.xml 2>/dev/null)

ifneq ($(MAKECMDGOALS),clean)
    -include $(DEPS)
endif


M1M3_CPPFLAGS := -I"/usr/include" \
	-I"../src" \
	-I"../src/LSST/M1M3/SS/DigitalInputOutput" \
	-I"../src/LSST/M1M3/SS/FirmwareUpdate" \
	-I"../src/LSST/M1M3/SS/Accelerometer" \
	-I"../src/LSST/M1M3/SS/BusLists" \
	-I"../src/LSST/M1M3/SS/CommandFactory" \
	-I"../src/LSST/M1M3/SS/Displacement" \
	-I"../src/LSST/M1M3/SS/Inclinometer" \
	-I"../src/LSST/M1M3/SS/ForceComponents" \
	-I"../src/LSST/M1M3/SS/Commands" \
	-I"../src/LSST/M1M3/SS/Context" \
	-I"../src/LSST/M1M3/SS/Controllers" \
	-I"../src/LSST/M1M3/SS/Domain" \
	-I"../src/LSST/M1M3/SS/FPGA" \
	-I"../src/LSST/M1M3/SS/Gyro" \
	-I"../src/LSST/M1M3/SS/ILC" \
	-I"../src/LSST/M1M3/SS/Include" \
	-I"../src/LSST/M1M3/SS/Logging" \
	-I"../src/LSST/M1M3/SS/Modbus" \
	-I"../src/LSST/M1M3/SS/Model" \
	-I"../src/LSST/M1M3/SS/PID" \
	-I"../src/LSST/M1M3/SS/Publisher" \
	-I"../src/LSST/M1M3/SS/Settings" \
	-I"../src/LSST/M1M3/SS/StateFactory" \
	-I"../src/LSST/M1M3/SS/States" \
	-I"../src/LSST/M1M3/SS/Subscriber" \
	-I"../src/LSST/M1M3/SS/Threads" \
	-I"../src/LSST/M1M3/SS/Utility" \

compile: $(BINARIES)

run: compile
	@$(foreach b,$(BINARIES),echo '[RUN] ${b}'; ./${b};)

junit: compile
	@$(foreach b,$(BINARIES),echo '[JUT] ${b}'; ./${b} -r junit -o ${b}.xml;)

clean:
	@$(foreach df,$(BINARIES) $(DEPS) $(JUNIT_FILES),echo '[RM ] ${df}'; $(RM) ${df};)

../src/libM1M3SS.a: FORCE
	@$(MAKE) -C ../src libM1M3SS.a SIMULATOR=1

%.cpp.o: %.cpp.d
	@echo '[CPP] $(patsubst %.d,%,$<)'
	${co}$(CPP) $(BOOST_CPPFLAGS) $(SAL_CPPFLAGS) $(M1M3_CPPFLAGS) -c -fmessage-length=0 -o $@ $(patsubst %.d,%,$<)

%.cpp.d: %.cpp
	@echo '[DPP] $<'
	${co}$(CPP) $(BOOST_CPPFLAGS) $(SAL_CPPFLAGS) $(M1M3_CPPFLAGS) -M $< -MF $@ -MT '$(patsubst %.cpp,%.o,$<) $@'

${BINARIES}: %: %.cpp.o ../src/libM1M3SS.a
	@echo '[TPP] $<'
	${co}$(CPP) -o $@ $(LIBS_FLAGS) $(BOOST_CPPFLAGS) $(SAL_CPPFLAGS) $(M1M3_CPPFLAGS) $^ ../$(CRIOCPP)/lib/libcRIOcpp.a ${LIBS}
