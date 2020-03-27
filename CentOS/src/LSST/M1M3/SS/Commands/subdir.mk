# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
src/LSST/M1M3/SS/Commands/AbortProfileCommand.cpp \
src/LSST/M1M3/SS/Commands/AbortRaiseM1M3Command.cpp \
src/LSST/M1M3/SS/Commands/ApplyAberrationForcesByBendingModesCommand.cpp \
src/LSST/M1M3/SS/Commands/ApplyAberrationForcesCommand.cpp \
src/LSST/M1M3/SS/Commands/ApplyActiveOpticForcesByBendingModesCommand.cpp \
src/LSST/M1M3/SS/Commands/ApplyActiveOpticForcesCommand.cpp \
src/LSST/M1M3/SS/Commands/ApplyOffsetForcesByMirrorForceCommand.cpp \
src/LSST/M1M3/SS/Commands/ApplyOffsetForcesCommand.cpp \
src/LSST/M1M3/SS/Commands/BootCommand.cpp \
src/LSST/M1M3/SS/Commands/ClearAberrationForcesCommand.cpp \
src/LSST/M1M3/SS/Commands/ClearActiveOpticForcesCommand.cpp \
src/LSST/M1M3/SS/Commands/ClearOffsetForcesCommand.cpp \
src/LSST/M1M3/SS/Commands/Command.cpp \
src/LSST/M1M3/SS/Commands/DisableCommand.cpp \
src/LSST/M1M3/SS/Commands/DisableHardpointChaseCommand.cpp \
src/LSST/M1M3/SS/Commands/DisableHardpointCorrectionsCommand.cpp \
src/LSST/M1M3/SS/Commands/EnableCommand.cpp \
src/LSST/M1M3/SS/Commands/EnableHardpointChaseCommand.cpp \
src/LSST/M1M3/SS/Commands/EnableHardpointCorrectionsCommand.cpp \
src/LSST/M1M3/SS/Commands/EnterEngineeringCommand.cpp \
src/LSST/M1M3/SS/Commands/ExitEngineeringCommand.cpp \
src/LSST/M1M3/SS/Commands/LowerM1M3Command.cpp \
src/LSST/M1M3/SS/Commands/ModbusTransmitCommand.cpp \
src/LSST/M1M3/SS/Commands/MoveHardpointActuatorsCommand.cpp \
src/LSST/M1M3/SS/Commands/PositionM1M3Command.cpp \
src/LSST/M1M3/SS/Commands/ProgramILCCommand.cpp \
src/LSST/M1M3/SS/Commands/RaiseM1M3Command.cpp \
src/LSST/M1M3/SS/Commands/ResetPIDCommand.cpp \
src/LSST/M1M3/SS/Commands/RunMirrorForceProfileCommand.cpp \
src/LSST/M1M3/SS/Commands/ShutdownCommand.cpp \
src/LSST/M1M3/SS/Commands/StandbyCommand.cpp \
src/LSST/M1M3/SS/Commands/StartCommand.cpp \
src/LSST/M1M3/SS/Commands/StopHardpointMotionCommand.cpp \
src/LSST/M1M3/SS/Commands/TMAAzimuthSampleCommand.cpp \
src/LSST/M1M3/SS/Commands/TMAElevationSampleCommand.cpp \
src/LSST/M1M3/SS/Commands/TestAirCommand.cpp \
src/LSST/M1M3/SS/Commands/TestForceActuatorCommand.cpp \
src/LSST/M1M3/SS/Commands/TestHardpointCommand.cpp \
src/LSST/M1M3/SS/Commands/TranslateM1M3Command.cpp \
src/LSST/M1M3/SS/Commands/TurnAirOffCommand.cpp \
src/LSST/M1M3/SS/Commands/TurnAirOnCommand.cpp \
src/LSST/M1M3/SS/Commands/TurnLightsOffCommand.cpp \
src/LSST/M1M3/SS/Commands/TurnLightsOnCommand.cpp \
src/LSST/M1M3/SS/Commands/TurnPowerOffCommand.cpp \
src/LSST/M1M3/SS/Commands/TurnPowerOnCommand.cpp \
src/LSST/M1M3/SS/Commands/UpdateCommand.cpp \
src/LSST/M1M3/SS/Commands/UpdatePIDCommand.cpp 

# Each subdirectory must supply rules for building sources it contributes
src/LSST/M1M3/SS/Commands/%.o: ../src/LSST/M1M3/SS/Commands/%.cpp
	@echo '[CPP] $<'
	${co}$(CPP) $(BOOST_CPPFLAGS) $(SAL_CPPFLAGS) -I"../src/LSST/M1M3/SS/DigitalInputOutput" -I"../src/LSST/M1M3/SS/FirmwareUpdate" -I"../src/LSST/M1M3/SS/Accelerometer" -I"../src/LSST/M1M3/SS/AutomaticOperationsController" -I"../src/LSST/M1M3/SS/BusLists" -I"../src/LSST/M1M3/SS/CommandFactory" -I"../src/LSST/M1M3/SS/Displacement" -I"../src/LSST/M1M3/SS/Inclinometer" -I"../src/LSST/M1M3/SS/ForceController" -I"../src/LSST/M1M3/SS/Commands" -I"../src/LSST/M1M3/SS/Context" -I"../src/LSST/M1M3/SS/Controller" -I"../src/LSST/M1M3/SS/Domain" -I"../src/LSST/M1M3/SS/FPGA" -I"../src/LSST/M1M3/SS/Gyro" -I"../src/LSST/M1M3/SS/ILC" -I"../src/LSST/M1M3/SS/Include" -I"../src/LSST/M1M3/SS/Logging" -I"../src/LSST/M1M3/SS/Modbus" -I"../src/LSST/M1M3/SS/Model" -I"../src/LSST/M1M3/SS/PID" -I"../src/LSST/M1M3/SS/PositionController" -I"../src/LSST/M1M3/SS/PowerController" -I"../src/LSST/M1M3/SS/Publisher" -I"../src/LSST/M1M3/SS/SafetyController" -I"../src/LSST/M1M3/SS/Settings" -I"../src/LSST/M1M3/SS/StateFactory" -I"../src/LSST/M1M3/SS/States" -I"../src/LSST/M1M3/SS/Subscriber" -I"../src/LSST/M1M3/SS/Threads" -I"../src/LSST/M1M3/SS/Utility" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
