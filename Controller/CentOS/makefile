################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/LSST/M1M3/SS/Utility/subdir.mk
-include src/LSST/M1M3/SS/Threads/subdir.mk
-include src/LSST/M1M3/SS/Subscriber/subdir.mk
-include src/LSST/M1M3/SS/States/subdir.mk
-include src/LSST/M1M3/SS/StateFactory/subdir.mk
-include src/LSST/M1M3/SS/Settings/subdir.mk
-include src/LSST/M1M3/SS/SafetyController/subdir.mk
-include src/LSST/M1M3/SS/Publisher/subdir.mk
-include src/LSST/M1M3/SS/PowerController/subdir.mk
-include src/LSST/M1M3/SS/PositionController/subdir.mk
-include src/LSST/M1M3/SS/PID/subdir.mk
-include src/LSST/M1M3/SS/Model/subdir.mk
-include src/LSST/M1M3/SS/Modbus/subdir.mk
-include src/LSST/M1M3/SS/Logging/subdir.mk
-include src/LSST/M1M3/SS/Include/subdir.mk
-include src/LSST/M1M3/SS/Inclinometer/subdir.mk
-include src/LSST/M1M3/SS/ILC/subdir.mk
-include src/LSST/M1M3/SS/Gyro/subdir.mk
-include src/LSST/M1M3/SS/ForceController/subdir.mk
-include src/LSST/M1M3/SS/FirmwareUpdate/subdir.mk
-include src/LSST/M1M3/SS/FPGA/subdir.mk
-include src/LSST/M1M3/SS/Domain/subdir.mk
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
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ts_M1M3Support

# Tool invocations
ts_M1M3Support: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L"../../../ts_sal/test" -L"../../../ts_sal/test/lib" -L"../../../ts_opensplice/OpenSpliceDDS/V6.4.1/HDE/x86_64.linux/lib" -L"../../../ts_sal/lsstsal/lib" -o "ts_M1M3Support" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) ts_M1M3Support
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
