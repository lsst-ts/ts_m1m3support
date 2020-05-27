include ../Makefile.inc

all: libM1M3SS.a

C_SRCS = $(shell find LSST -name '*.c')
CPP_SRCS = $(shell find LSST pugixml -name '*.cpp')

OBJS = $(patsubst %.c,%.o,$(C_SRCS)) $(patsubst %.cpp,%.o,$(CPP_SRCS))

C_DEPS = $(patsubst %.c,%.d,$(C_SRCS))
CPP_DEPS = $(patsubst %.cpp,%.d,$(CPP_SRCS))

M1M3_CPPFLAGS := -I. \
	-I"LSST/M1M3/SS/DigitalInputOutput" \
	-I"LSST/M1M3/SS/FirmwareUpdate" \
	-I"LSST/M1M3/SS/Accelerometer" \
	-I"LSST/M1M3/SS/AutomaticOperationsController" \
	-I"LSST/M1M3/SS/BusLists" \
	-I"LSST/M1M3/SS/CommandFactory" \
	-I"LSST/M1M3/SS/Displacement" \
	-I"LSST/M1M3/SS/Inclinometer" \
	-I"LSST/M1M3/SS/ForceController" \
	-I"LSST/M1M3/SS/Commands" \
	-I"LSST/M1M3/SS/Context" \
	-I"LSST/M1M3/SS/Controller" \
	-I"LSST/M1M3/SS/Domain" \
	-I"LSST/M1M3/SS/FPGA" \
	-I"LSST/M1M3/SS/Gyro" \
	-I"LSST/M1M3/SS/ILC" \
	-I"LSST/M1M3/SS/Include" \
	-I"LSST/M1M3/SS/Logging" \
	-I"LSST/M1M3/SS/Modbus" \
	-I"LSST/M1M3/SS/Model" \
	-I"LSST/M1M3/SS/PID" \
	-I"LSST/M1M3/SS/PositionController" \
	-I"LSST/M1M3/SS/PowerController" \
	-I"LSST/M1M3/SS/ProfileController" \
	-I"LSST/M1M3/SS/Publisher" \
	-I"LSST/M1M3/SS/SafetyController" \
	-I"LSST/M1M3/SS/Settings" \
	-I"LSST/M1M3/SS/StateFactory" \
	-I"LSST/M1M3/SS/States" \
	-I"LSST/M1M3/SS/Subscriber" \
	-I"LSST/M1M3/SS/Threads" \
	-I"LSST/M1M3/SS/Utility"

libM1M3SS.a: $(OBJS)
	@echo '[AR ] $@'
	${co}$(AR) r $@ $^

clean:
	@$(foreach file,$(OBJS) $(C_DEPS) $(CPP_DEPS) libM1M3SS.a $(shell find -name '*.d'), echo '[RM ] ${file}'; $(RM) -r $(file);)

# file targets
%.o: %.cpp
	@echo '[CPP] $^'
	${co}$(CPP) $(BOOST_CPPFLAGS) $(SAL_CPPFLAGS) $(M1M3_CPPFLAGS) -c -fmessage-length=0 -o $@ $^

%.o: %.c
	@echo '[C  ] $^'
	${co}$(C) -c -fmessage-length=0 -o $@ $^

