include ../Makefile.inc

all: run

.PHONY: run clean

TEST_SRCS = $(shell find -name '*.cpp')
BINARIES = $(patsubst %.cpp,%,$(TEST_SRCS))

M1M3_CPPFLAGS := -I../src -I"../src/LSST/M1M3/SS/DigitalInputOutput" -I"../src/LSST/M1M3/SS/FirmwareUpdate" -I"../src/LSST/M1M3/SS/Accelerometer" -I"../src/LSST/M1M3/SS/AutomaticOperationsController" -I"../src/LSST/M1M3/SS/BusLists" -I"../src/LSST/M1M3/SS/CommandFactory" -I"../src/LSST/M1M3/SS/Displacement" -I"../src/LSST/M1M3/SS/Inclinometer" -I"../src/LSST/M1M3/SS/ForceController" -I"../src/LSST/M1M3/SS/Commands" -I"../src/LSST/M1M3/SS/Context" -I"../src/LSST/M1M3/SS/Controller" -I"../src/LSST/M1M3/SS/Domain" -I"../src/LSST/M1M3/SS/FPGA" -I"../src/LSST/M1M3/SS/Gyro" -I"../src/LSST/M1M3/SS/ILC" -I"../src/LSST/M1M3/SS/Include" -I"../src/LSST/M1M3/SS/Logging" -I"../src/LSST/M1M3/SS/Modbus" -I"../src/LSST/M1M3/SS/Model" -I"../src/LSST/M1M3/SS/PID" -I"../src/LSST/M1M3/SS/PositionController" -I"../src/LSST/M1M3/SS/PowerController" -I"../src/LSST/M1M3/SS/ProfileController" -I"../src/LSST/M1M3/SS/Publisher" -I"../src/LSST/M1M3/SS/SafetyController" -I"../src/LSST/M1M3/SS/Settings" -I"../src/LSST/M1M3/SS/StateFactory" -I"../src/LSST/M1M3/SS/States" -I"../src/LSST/M1M3/SS/Subscriber" -I"../src/LSST/M1M3/SS/Threads" -I"../src/LSST/M1M3/SS/Utility"

run: $(BINARIES)
	$(foreach b,$^,./${b})

clean:
	$(RM) $(BINARIES)

%: %.cpp ../src/libM1M3SS.a
	@echo '[TPP] $?'
	${co}$(CPP) -o $@ $(LIBS_FLAGS) $(BOOST_CPPFLAGS) $(SAL_CPPFLAGS) $(LIBS) $(M1M3_CPPFLAGS) $^

../src/libM1M3SS.a:
	$(MAKE) -C ../src libM1M3SS.a SIMULATOR=1
