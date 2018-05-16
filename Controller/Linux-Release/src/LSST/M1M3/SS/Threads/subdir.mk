################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/LSST/M1M3/SS/Threads/ControllerThread.cpp \
../src/LSST/M1M3/SS/Threads/IThread.cpp \
../src/LSST/M1M3/SS/Threads/OuterLoopClockThread.cpp \
../src/LSST/M1M3/SS/Threads/PPSThread.cpp \
../src/LSST/M1M3/SS/Threads/SubscriberThread.cpp 

OBJS += \
./src/LSST/M1M3/SS/Threads/ControllerThread.o \
./src/LSST/M1M3/SS/Threads/IThread.o \
./src/LSST/M1M3/SS/Threads/OuterLoopClockThread.o \
./src/LSST/M1M3/SS/Threads/PPSThread.o \
./src/LSST/M1M3/SS/Threads/SubscriberThread.o 

CPP_DEPS += \
./src/LSST/M1M3/SS/Threads/ControllerThread.d \
./src/LSST/M1M3/SS/Threads/IThread.d \
./src/LSST/M1M3/SS/Threads/OuterLoopClockThread.d \
./src/LSST/M1M3/SS/Threads/PPSThread.d \
./src/LSST/M1M3/SS/Threads/SubscriberThread.d 


# Each subdirectory must supply rules for building sources it contributes
src/LSST/M1M3/SS/Threads/%.o: ../src/LSST/M1M3/SS/Threads/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/usr/boost_1_65_1" -I"/usr/ts_opensplice/OpenSpliceDDS/V6.7/HDE/x86_64.linux/examples/include" -I"/usr/ts_opensplice/OpenSpliceDDS/V6.7/HDE/x86_64.linux/examples" -I"/usr/ts_opensplice/OpenSpliceDDS/V6.7/HDE/x86_64.linux/include" -I"/usr/ts_opensplice/OpenSpliceDDS/V6.7/HDE/x86_64.linux/include/sys" -I"/usr/ts_opensplice/OpenSpliceDDS/V6.7/HDE/x86_64.linux/include/dcps/C++/SACPP" -I"/usr/ts_sal/test/m1m3/cpp/src" -I"/usr/ts_sal/test/m1m3/cpp" -I"/usr/ts_sal/test/MTMount/cpp/src" -I"/usr/ts_sal/lsstsal/include" -I"/usr/ts_sal/test/m1m3/cpp" -I"/usr/ts_sal/test/MTMount/cpp" -I"/usr/ts_sal/test" -I"/usr/ts_sal/test/include" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\DigitalInputOutput" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\FirmwareUpdate" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Accelerometer" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\AutomaticOperationsController" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\BusLists" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\CommandFactory" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Displacement" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Inclinometer" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\ForceController" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Commands" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Context" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Controller" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\FPGA" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Gyro" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\ILC" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Include" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Logging" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Modbus" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Model" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\PID" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\PositionController" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\PowerController" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\ProfileController" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Publisher" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\SafetyController" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Settings" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\StateFactory" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\States" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Subscriber" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Threads" -I"D:\Code\ts_M1M3Support\Controller\src\LSST\M1M3\SS\Utility" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


