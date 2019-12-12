# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/LSST/M1M3/SS/Settings/AliasApplicationSettings.cpp \
../src/LSST/M1M3/SS/Settings/ExpansionFPGAApplicationSettings.cpp \
../src/LSST/M1M3/SS/Settings/ForceActuatorApplicationSettings.cpp \
../src/LSST/M1M3/SS/Settings/ForceActuatorSettings.cpp \
../src/LSST/M1M3/SS/Settings/HardpointActuatorApplicationSettings.cpp \
../src/LSST/M1M3/SS/Settings/HardpointActuatorSettings.cpp \
../src/LSST/M1M3/SS/Settings/HardpointMonitorApplicationSettings.cpp \
../src/LSST/M1M3/SS/Settings/ILCApplicationSettings.cpp \
../src/LSST/M1M3/SS/Settings/InterlockApplicationSettings.cpp \
../src/LSST/M1M3/SS/Settings/PIDSettings.cpp \
../src/LSST/M1M3/SS/Settings/PositionControllerSettings.cpp \
../src/LSST/M1M3/SS/Settings/RecommendedApplicationSettings.cpp \
../src/LSST/M1M3/SS/Settings/SafetyControllerSettings.cpp \
../src/LSST/M1M3/SS/Settings/SettingReader.cpp \
../src/LSST/M1M3/SS/Settings/Settings.cpp 

OBJS += \
./src/LSST/M1M3/SS/Settings/AliasApplicationSettings.o \
./src/LSST/M1M3/SS/Settings/ExpansionFPGAApplicationSettings.o \
./src/LSST/M1M3/SS/Settings/ForceActuatorApplicationSettings.o \
./src/LSST/M1M3/SS/Settings/ForceActuatorSettings.o \
./src/LSST/M1M3/SS/Settings/HardpointActuatorApplicationSettings.o \
./src/LSST/M1M3/SS/Settings/HardpointActuatorSettings.o \
./src/LSST/M1M3/SS/Settings/HardpointMonitorApplicationSettings.o \
./src/LSST/M1M3/SS/Settings/ILCApplicationSettings.o \
./src/LSST/M1M3/SS/Settings/InterlockApplicationSettings.o \
./src/LSST/M1M3/SS/Settings/PIDSettings.o \
./src/LSST/M1M3/SS/Settings/PositionControllerSettings.o \
./src/LSST/M1M3/SS/Settings/RecommendedApplicationSettings.o \
./src/LSST/M1M3/SS/Settings/SafetyControllerSettings.o \
./src/LSST/M1M3/SS/Settings/SettingReader.o \
./src/LSST/M1M3/SS/Settings/Settings.o 

CPP_DEPS += \
./src/LSST/M1M3/SS/Settings/AliasApplicationSettings.d \
./src/LSST/M1M3/SS/Settings/ExpansionFPGAApplicationSettings.d \
./src/LSST/M1M3/SS/Settings/ForceActuatorApplicationSettings.d \
./src/LSST/M1M3/SS/Settings/ForceActuatorSettings.d \
./src/LSST/M1M3/SS/Settings/HardpointActuatorApplicationSettings.d \
./src/LSST/M1M3/SS/Settings/HardpointActuatorSettings.d \
./src/LSST/M1M3/SS/Settings/HardpointMonitorApplicationSettings.d \
./src/LSST/M1M3/SS/Settings/ILCApplicationSettings.d \
./src/LSST/M1M3/SS/Settings/InterlockApplicationSettings.d \
./src/LSST/M1M3/SS/Settings/PIDSettings.d \
./src/LSST/M1M3/SS/Settings/PositionControllerSettings.d \
./src/LSST/M1M3/SS/Settings/RecommendedApplicationSettings.d \
./src/LSST/M1M3/SS/Settings/SafetyControllerSettings.d \
./src/LSST/M1M3/SS/Settings/SettingReader.d \
./src/LSST/M1M3/SS/Settings/Settings.d 


# Each subdirectory must supply rules for building sources it contributes
src/LSST/M1M3/SS/Settings/%.o: ../src/LSST/M1M3/SS/Settings/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -pedantic -I"../../../boost_1_65_1" -I"../../../ts_opensplice/OpenSpliceDDS/V6.9/HDE/x86_64.linux/include" -I"../../../ts_opensplice/OpenSpliceDDS/V6.9/HDE/x86_64.linux/include/sys" -I"../../../ts_opensplice/OpenSpliceDDS/V6.9/HDE/x86_64.linux/include/dcps/C++/SACPP" -I"../../../ts_sal/test/MTM1M3/cpp/src" -I"../../../ts_sal/test/MTM1M3/cpp" -I"../../../ts_sal/test/MTMount/cpp/src" -I"../../../ts_sal/lsstsal/include" -I"../../../ts_sal/test/MTMount/cpp" -I"../../../ts_sal/test" -I../../../ts_sal/test/include -I"../src/LSST/M1M3/SS/DigitalInputOutput" -I"../src/LSST/M1M3/SS/FirmwareUpdate" -I"../src/LSST/M1M3/SS/Accelerometer" -I"../src/LSST/M1M3/SS/AutomaticOperationsController" -I"../src/LSST/M1M3/SS/BusLists" -I"../src/LSST/M1M3/SS/CommandFactory" -I"../src/LSST/M1M3/SS/Displacement" -I"../src/LSST/M1M3/SS/Inclinometer" -I"../src/LSST/M1M3/SS/ForceController" -I"../src/LSST/M1M3/SS/Commands" -I"../src/LSST/M1M3/SS/Context" -I"../src/LSST/M1M3/SS/Controller" -I"../src/LSST/M1M3/SS/Domain" -I"../src/LSST/M1M3/SS/FPGA" -I"../src/LSST/M1M3/SS/Gyro" -I"../src/LSST/M1M3/SS/ILC" -I"../src/LSST/M1M3/SS/Include" -I"../src/LSST/M1M3/SS/Logging" -I"../src/LSST/M1M3/SS/Modbus" -I"../src/LSST/M1M3/SS/Model" -I"../src/LSST/M1M3/SS/PID" -I"../src/LSST/M1M3/SS/PositionController" -I"../src/LSST/M1M3/SS/PowerController" -I"../src/LSST/M1M3/SS/Publisher" -I"../src/LSST/M1M3/SS/SafetyController" -I"../src/LSST/M1M3/SS/Settings" -I"../src/LSST/M1M3/SS/StateFactory" -I"../src/LSST/M1M3/SS/States" -I"../src/LSST/M1M3/SS/Subscriber" -I"../src/LSST/M1M3/SS/Threads" -I"../src/LSST/M1M3/SS/Utility" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


