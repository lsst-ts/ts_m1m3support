ifndef VERBOSE
  co := @
endif

-include ../makefile.init

# define to "" to produce verbose output

C := gcc -O3 -Wall -g
CPP := g++ -std=c++11 -pedantic -O3 -Wall -g

ifdef SIMULATOR
  CPP += -DSIMULATOR
endif

BOOST_CPPFLAGS := -I/usr/include/boost169
SAL_CPPFLAGS := -I${OSPL_HOME}/include -I${OSPL_HOME}/include/sys -I${OSPL_HOME}/include/dcps/C++/SACPP -I${SAL_WORK_DIR}/MTM1M3/cpp/src -I${SAL_WORK_DIR}/MTMount/cpp/src -I${SAL_WORK_DIR}/include

# All of the sources participating in the build are defined here
-include sources.mk
-include src/LSST/M1M3/SS/Utility/subdir.mk
-include src/LSST/M1M3/SS/Include/subdir.mk
-include src/LSST/M1M3/SS/Threads/subdir.mk
-include src/LSST/M1M3/SS/Subscriber/subdir.mk
-include src/LSST/M1M3/SS/States/subdir.mk
-include src/LSST/M1M3/SS/StateFactory/subdir.mk
-include src/LSST/M1M3/SS/Settings/subdir.mk
-include src/LSST/M1M3/SS/SafetyController/subdir.mk
-include src/LSST/M1M3/SS/Publisher/subdir.mk
-include src/LSST/M1M3/SS/ProfileController/subdir.mk
-include src/LSST/M1M3/SS/PowerController/subdir.mk
-include src/LSST/M1M3/SS/PositionController/subdir.mk
-include src/LSST/M1M3/SS/PID/subdir.mk
-include src/LSST/M1M3/SS/Model/subdir.mk
-include src/LSST/M1M3/SS/Modbus/subdir.mk
-include src/LSST/M1M3/SS/Logging/subdir.mk
-include src/LSST/M1M3/SS/Inclinometer/subdir.mk
-include src/LSST/M1M3/SS/ILC/subdir.mk
-include src/LSST/M1M3/SS/Gyro/subdir.mk
-include src/LSST/M1M3/SS/ForceController/subdir.mk
-include src/LSST/M1M3/SS/FirmwareUpdate/subdir.mk
-include src/LSST/M1M3/SS/FPGA/subdir.mk
-include src/LSST/M1M3/SS/Displacement/subdir.mk
-include src/LSST/M1M3/SS/DigitalInputOutput/subdir.mk
-include src/LSST/M1M3/SS/Controller/subdir.mk
-include src/LSST/M1M3/SS/Context/subdir.mk
-include src/LSST/M1M3/SS/Commands/subdir.mk
-include src/LSST/M1M3/SS/CommandFactory/subdir.mk
-include src/LSST/M1M3/SS/BusLists/subdir.mk
-include src/LSST/M1M3/SS/AutomaticOperationsController/subdir.mk
-include src/LSST/M1M3/SS/Accelerometer/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

.PHONY: all clean dependents

OBJS = $(patsubst %.c,%.o,$(C_SRCS)) $(patsubst %.cpp,%.o,$(CPP_SRCS))

C_DEPS = $(patsubst %.c,%.d,$(C_SRCS))
CPP_DEPS = $(patsubst %.cpp,%.d,$(CPP_SRCS))

# Add inputs and outputs from these tool invocations to the build variables 
#

# All Target
all: ts_M1M3Support

# Tool invocations
ts_M1M3Support: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	g++ -L"${SAL_WORK_DIR}/lib" -L"${OSPL_HOME}/lib" -o "ts_M1M3Support" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	$(foreach file,$(OBJS) $(C_DEPS) $(CPP_DEPS) ts_M1M3Support, echo '[RM ] ${file}'; $(RM) -r $(file);)

-include ../makefile.targets
