# M1M3 failure codes

Unless noted, setting override are in SafetyControllerSettings section of the init file. Faults codes

## No Fault - 0x00000000 (0)

Obviously meaning no fault. Used to reset previous error codes.

## [Air Valve (mask 0x00010000, range 65537 - 65538)](fault_codes/air_valve.md)

## [Displacement Sensors - Independent Measurement System (IMS) (mask 0x00020000, range 131073 - 131086](fault_codes/displacement_sensors.md)

## [Inclinometer (mask 0x00030000, range 196609 - 196616)](fault_codes/inclinometer.md)

## [Interlock (mask 0x00040000, range 262145 - 262156)](fault_codes/interlock.md)

## [Force Controller (mask 0x00050000, range 327681 - 327701)](fault_codes/force_controller.md)

## [Cell Light Sensor (mask 0x00060000, range 393217 - 323218)](fault_codes/cell_light.md)

## PowerControllerPowerNetworkAOutputMismatch - 0x00070001 (458753)

### Severity

Hardware Power Fault.

### Rectification

Commanded status for Power Network A does not match feedback contactor relay. Check 24V supply fuses and cRIO DO/DI module wiring for Sub-network A.

### Telemetry

MTM1M3\_logevent\_powerSupplyFault | powerNetworkAOutputMismatch

### Setting override

PowerControllerSettings | FaultOnPowerNetworkAOutputMismatch

## PowerControllerPowerNetworkBOutputMismatch - 0x00070002 (458754)

### Severity

Hardware Power Fault.

### Rectification

Check contactors, circuit breakers, and digital feedback lines for Power Network B.

### Telemetry

MTM1M3\_logevent\_powerSupplyFault | powerNetworkBOutputMismatch

### Setting override

PowerControllerSettings | FaultOnPowerNetworkBOutputMismatch

## PowerControllerPowerNetworkCOutputMismatch - 0x00070003 (458755)

### Severity

Hardware Power Fault.

### Rectification

Check contactors, circuit breakers, and digital feedback lines for Power Network C.

### Telemetry

MTM1M3\_logevent\_powerSupplyFault | powerNetworkCOutputMismatch

### Setting override

PowerControllerSettings | FaultOnPowerNetworkCOutputMismatch

## PowerControllerPowerNetworkDOutputMismatch - 0x00070004 (458756)

### Severity

Hardware Power Fault.

### Rectification

Check contactors, circuit breakers, and digital feedback lines for Power Network D.

### Telemetry

MTM1M3\_logevent\_powerSupplyFault | powerNetworkDOutputMismatch

### Setting override

PowerControllerSettings | FaultOnPowerNetworkDOutputMismatch

## PowerControllerAuxPowerNetworkAOutputMismatch - 0x00070005 (458757)

### Severity

Hardware Power Fault.

### Rectification

Inspect auxiliary power bus A distribution panel, switching relays, and monitor input lines.

### Telemetry

MTM1M3\_logevent\_powerSupplyFault | auxPowerNetworkAOutputMismatch

### Setting override

PowerControllerSettings | FaultOnAuxPowerNetworkAOutputMismatch

## PowerControllerAuxPowerNetworkBOutputMismatch - 0x00070006 (458758)

### Severity

Hardware Power Fault.

### Rectification

Inspect auxiliary power bus B distribution panel, switching relays, and monitor input lines.

### Telemetry

MTM1M3\_logevent\_powerSupplyFault | auxPowerNetworkBOutputMismatch

### Setting override

PowerControllerSettings | FaultOnAuxPowerNetworkBOutputMismatch

## PowerControllerAuxPowerNetworkCOutputMismatch - 0x00070007 (458759)

### Severity

Hardware Power Fault.

### Rectification

Inspect auxiliary power bus C distribution panel, switching relays, and monitor input lines.

### Telemetry

MTM1M3\_logevent\_powerSupplyFault | auxPowerNetworkCOutputMismatch

### Setting override

PowerControllerSettings | FaultOnAuxPowerNetworkCOutputMismatch

## PowerControllerAuxPowerNetworkDOutputMismatch - 0x00070008 (458760)

### Severity

Hardware Power Fault.

### Rectification

Inspect auxiliary power bus D distribution panel, switching relays, and monitor input lines.

### Telemetry

MTM1M3\_logevent\_powerSupplyFault | auxPowerNetworkDOutputMismatch

### Setting override

PowerControllerSettings | FaultOnAuxPowerNetworkDOutputMismatch

## RaiseOperationTimeout - 0x00080001 (524289)

### Severity

Operational Fault - Raise operation aborted.

### Rectification

Mirror raise sequence took longer than maximum configured timeout. Check pneumatic system pressure build rate and hardpoint motion progress.

### Telemetry

MTM1M3\_logevent\_supportOperationFault | raiseOperationTimeout

### Setting override

SupportOperationSettings | FaultOnRaiseOperationTimeout

## LowerOperationTimeout - 0x00080002 (524290)

### Severity

Operational Fault - Lower operation aborted.

### Rectification

Mirror lower sequence failed to reach static parked state within time limit. Check pneumatic bleed rates and hardpoint retraction status.

### Telemetry

MTM1M3\_logevent\_supportOperationFault | lowerOperationTimeout

### Setting override

SupportOperationSettings | FaultOnLowerOperationTimeout

## ILCCommunicationTimeout - 0x00080003 (524291)

### Severity

Critical Communication Fault - Actuator control disrupted.

### Rectification

Check subnet communication loops to Individual Loop Controllers (ILC). Verify power and bus connections to subnet Modbus/RS485 master nodes.

### Telemetry

MTM1M3\_logevent\_ilcWarning | communicationTimeout

### Setting override

ILCSettings | FaultOnCommunicationTimeout

## ModbusIRQTimeout - 0x00080004 (524292)

### Severity

System Timing / Communication Fault.

### Rectification

Interrupt processing timeout on cRIO Modbus interface module. Check CPU utilization on cRIO and inspect serial traffic for dropped interrupt signals.

### Telemetry

MTM1M3\_logevent\_ilcWarning | modbusIRQTimeout

### Setting override

ILCSettings | FaultOnModbusIRQTimeout

## ForceActuatorFollowingErrorCounting - 0x00090001 (589825)

### Severity

Actuator Performance Warning / Fault.

### Rectification

Force actuator output feedback deviates from command setpoint continuously. Inspect specific pneumatic valve response and calibration parameters.

### Telemetry

MTM1M3\_logevent\_forceActuatorWarning | followingErrorCounting

### Setting override

ForceActuatorSettings | FaultOnFollowingErrorCounting

## ForceActuatorFollowingErrorImmediate - 0x00090002 (589826)

### Severity

Critical Actuator Fault - Immediate shutdown of affected node/subsystem.

### Rectification

Large sudden step error between commanded and measured force on actuator. Check for line pressure drop, disconnected load cell cabling, or failed valve controller.

### Telemetry

MTM1M3\_logevent\_forceActuatorFault | followingErrorImmediate

### Setting override

ForceActuatorSettings | FaultOnFollowingErrorImmediate

## HardpointActuator - 0x000A0001 (655361)

### Severity

General Hardpoint Fault.

### Rectification

General fault reported on hardpoint positioning mechanism. Check encoder feedback, drive motor power, and internal health registers.

### Telemetry

MTM1M3\_logevent\_hardpointActuatorFault | hardpointActuator

### Setting override

HardpointSettings | FaultOnHardpointActuator

## HardpointActuatorLoadCellError - 0x000A0002 (655362)

### Severity

Critical Hardpoint Fault.

### Rectification

Load cell reading on target hardpoint is out of valid electrical/physical bounds. Check strain gauge excitation voltage, wiring, and ADC readout module.

### Telemetry

MTM1M3\_logevent\_hardpointActuatorFault | loadCellError

### Setting override

HardpointSettings | FaultOnLoadCellError

## HardpointActuatorMeasuredForceError - 0x000A0003 (655363)

### Severity

Critical Hardpoint Fault.

### Rectification

Measured force on hardpoint exceeds safe operational envelope during active optics mode. Verify load sharing between force actuators and hardpoints.

### Telemetry

MTM1M3\_logevent\_hardpointActuatorFault | measuredForceError

### Setting override

HardpointSettings | FaultOnMeasuredForceError

## HardpointActuatorAirPressureHigh - 0x000A0004 (655364)

## HardpointActuatorAirPressureLow - 0x000A0005 (655365)

## HardpointActuatorAirPressureOutside - 0x000A0006 (655366)

## HardpointActuatorLimitLowError - 0x000A0007 (655367)

## HardpointActuatorLimitHighError - 0x000A0008 (655368)

## HardpointActuatorFollowingError - 0x000A0009 (655369)

## HardpointUnstableError - 0x000A000A (6553670)

## HardpointHighTension - 0x000A000B (655371)

### Severity

Critical Safety Fault - Prevents mirror damage from excessive tensile loads.

### Rectification

Hardpoint load cell registered tension force exceeding maximum tension limit. Immediately verify mirror force balance and lower active optical forces.

### Telemetry

MTM1M3\_logevent\_hardpointActuatorFault | highTension

### Setting override

HardpointSettings | FaultOnHighTension

## TMAAzimuthTimeout - 0x000B0001 (720897)

## TMAElevationTimeout - 0x000B0002 (720898)

## TMAInclinometerDeviation - 0x000B0004 (720900)

## UserPanic - 0x000C0001 (786433)
