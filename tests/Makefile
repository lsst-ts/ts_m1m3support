include ../Makefile.inc

SUFFIXES += .d

all: compile

.PHONY: FORCE compile run junit clean

TEST_SRCS := $(shell ls test_*.cpp 2>/dev/null)
BINARIES := $(patsubst %.cpp,%,$(TEST_SRCS))
DEPS := $(patsubst %.cpp,%.d,$(TEST_SRCS))
OBJS := $(patsubst %.cpp,%.o,$(TEST_SRCS))
JUNIT_FILES := $(shell ls *.xml 2>/dev/null)

ifeq (0, $(words $(findstring $(MAKECMDGOALS), "clean")))
    -include $(DEPS)
endif


M1M3_CPPFLAGS := -I../src \
	-I"../src/LSST/M1M3/SS/DigitalInputOutput" \
	-I"../src/LSST/M1M3/SS/FirmwareUpdate" \
	-I"../src/LSST/M1M3/SS/Accelerometer" \
	-I"../src/LSST/M1M3/SS/AutomaticOperationsController" \
	-I"../src/LSST/M1M3/SS/BusLists" \
	-I"../src/LSST/M1M3/SS/CommandFactory" \
	-I"../src/LSST/M1M3/SS/Displacement" \
	-I"../src/LSST/M1M3/SS/Inclinometer" \
	-I"../src/LSST/M1M3/SS/ForceController" \
	-I"../src/LSST/M1M3/SS/Commands" \
	-I"../src/LSST/M1M3/SS/Context" \
	-I"../src/LSST/M1M3/SS/Controller" \
	-I"../src/LSST/M1M3/SS/Domain" \
	-I"../src/LSST/M1M3/SS/FPGA" \
	-I"../src/LSST/M1M3/SS/Gyro" \
	-I"../src/LSST/M1M3/SS/ILC" \
	-I"../src/LSST/M1M3/SS/Include" \
	-I"../src/LSST/M1M3/SS/Logging" \
	-I"../src/LSST/M1M3/SS/Modbus" \
	-I"../src/LSST/M1M3/SS/Model" \
	-I"../src/LSST/M1M3/SS/PID" \
	-I"../src/LSST/M1M3/SS/PositionController" \
	-I"../src/LSST/M1M3/SS/PowerController" \
	-I"../src/LSST/M1M3/SS/ProfileController" \
	-I"../src/LSST/M1M3/SS/Publisher" \
	-I"../src/LSST/M1M3/SS/SafetyController" \
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
	@$(foreach df,$(BINARIES) $(DEPS) $(OBJS) $(JUNIT_FILES),echo '[RM ] ${df}'; $(RM) ${df};)

../src/libM1M3SS.a: FORCE
	@$(MAKE) -C ../src libM1M3SS.a SIMULATOR=1

%.d: %.cpp
	@echo '[DEP] $<'
	${co}$(CPP) $(BOOST_CPPFLAGS) $(SAL_CPPFLAGS) $(M1M3_CPPFLAGS) -MM -MT '$(patsubst %.cpp,%.o,$<)' $< -MF $@

%: %.cpp ../src/libM1M3SS.a %.d
	@echo '[TPP] $<'
	${co}$(CPP) -o $@ $(LIBS_FLAGS) $(BOOST_CPPFLAGS) $(SAL_CPPFLAGS) $(LIBS) $(M1M3_CPPFLAGS) $< ../src/libM1M3SS.a
