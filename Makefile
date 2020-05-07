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

M1M3_CPPFLAGS := -I"src" -I"src/LSST/M1M3/SS/DigitalInputOutput" -I"src/LSST/M1M3/SS/FirmwareUpdate" -I"src/LSST/M1M3/SS/Accelerometer" -I"src/LSST/M1M3/SS/AutomaticOperationsController" -I"src/LSST/M1M3/SS/BusLists" -I"src/LSST/M1M3/SS/CommandFactory" -I"src/LSST/M1M3/SS/Displacement" -I"src/LSST/M1M3/SS/Inclinometer" -I"src/LSST/M1M3/SS/ForceController" -I"src/LSST/M1M3/SS/Commands" -I"src/LSST/M1M3/SS/Context" -I"src/LSST/M1M3/SS/Controller" -I"src/LSST/M1M3/SS/Domain" -I"src/LSST/M1M3/SS/FPGA" -I"src/LSST/M1M3/SS/Gyro" -I"src/LSST/M1M3/SS/ILC" -I"src/LSST/M1M3/SS/Include" -I"src/LSST/M1M3/SS/Logging" -I"src/LSST/M1M3/SS/Modbus" -I"src/LSST/M1M3/SS/Model" -I"src/LSST/M1M3/SS/PID" -I"src/LSST/M1M3/SS/PositionController" -I"src/LSST/M1M3/SS/PowerController" -I"src/LSST/M1M3/SS/ProfileController" -I"src/LSST/M1M3/SS/Publisher" -I"src/LSST/M1M3/SS/SafetyController" -I"src/LSST/M1M3/SS/Settings" -I"src/LSST/M1M3/SS/StateFactory" -I"src/LSST/M1M3/SS/States" -I"src/LSST/M1M3/SS/Subscriber" -I"src/LSST/M1M3/SS/Threads" -I"src/LSST/M1M3/SS/Utility" 

LIBS := -ldl -lSAL_MTMount -lSAL_MTM1M3 -lsacpp_MTMount_types -lsacpp_MTM1M3_types -ldcpssacpp -ldcpsgapi -lddsuser -lddskernel -lpthread -lddsserialization -lddsconfparser -lddsconf -lddsdatabase -lddsutil -lddsos

.PHONY: all clean dependents deploy

C_SRCS = $(shell find src/*/ -name '*.c')
CPP_SRCS = $(shell find src/*/ -name '*.cpp')

OBJS = $(patsubst %.c,%.o,$(C_SRCS)) $(patsubst %.cpp,%.o,$(CPP_SRCS))

C_DEPS = $(patsubst %.c,%.d,$(C_SRCS))
CPP_DEPS = $(patsubst %.cpp,%.d,$(CPP_SRCS))

# Add inputs and outputs from these tool invocations to the build variables 
#

# All Target
all: ts_M1M3Support $(m1m3cli)

# Tool invocations
ts_M1M3Support: src/ts_M1M3Support.o $(filter-out src/cliapp/%,$(OBJS))
	@echo '[LD ] $@'
	${co}$(CPP) -L"${SAL_WORK_DIR}/lib" -L"${OSPL_HOME}/lib" -L"${LSST_SDK_INSTALL}/lib" -o "$@" $? $(LIBS)

M1M3_OBJS = src/cliapp/CliApp.o \
  src/LSST/M1M3/SS/FPGA/FPGA.o \
  src/LSST/M1M3/SS/FPGA/NiFpga.o \
  src/LSST/M1M3/SS/FPGA/NiStatus.o \
  src/LSST/M1M3/SS/FPGA/NiError.o \
  src/LSST/M1M3/SS/FPGA/IFPGA.o \
  src/LSST/M1M3/SS/FPGA/FPGA.o \
  src/LSST/M1M3/SS/FPGA/HealthAndStatusFPGAData.o \
  src/LSST/M1M3/SS/Utility/U8ArrayUtilities.o \
  src/LSST/M1M3/SS/Utility/U16ArrayUtilities.o

m1m3cli: src/m1m3cli.o $(M1M3_OBJS)
	@echo '[LD ] $@'
	${co}$(CPP) -o "$@" $? -lreadline -ldl

# Other Targets
clean:
	$(foreach file,$(OBJS) $(C_DEPS) $(CPP_DEPS) ts_M1M3Support src/ts_M1M3Support.o src/m1m3cli.o, echo '[RM ] ${file}'; $(RM) -r $(file);)

# file targets
src/%.o: src/%.cpp
	@echo '[CPP] $<'
	${co}$(CPP) $(BOOST_CPPFLAGS) $(SAL_CPPFLAGS) $(M1M3_CPPFLAGS) -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"

src/%.o: src/%.c
	@echo '[C  ] $<'
	${co}$(C) -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"

CRIO_IP = 10.0.0.11

deploy: ts_M1M3Support m1m3cli
	@echo '[SCP] $?'
	${co}scp $? admin@${CRIO_IP}:
	@echo '[SCP] Bitfiles/NiFpga_M1M3SupportFPGA.lvbitx'
	${co}scp Bitfiles/NiFpga_M1M3SupportFPGA.lvbitx admin@${CRIO_IP}:Bitfiles
