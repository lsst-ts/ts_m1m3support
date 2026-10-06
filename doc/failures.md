# M1M3 failure codes

Unless noted, setting override are in SafetyControllerSettings section of the init file. Faults codes

## No Fault - 0x00000000 (0)

Obviously meaning no fault. Used to reset previous error codes.

## [Air Valve (mask 0x00010000, range 65537 - 65538)](fault_codes/air_valve.md)

## [Displacement Sensors - Independent Measurement System (IMS) (mask 0x00020000, range 131073 - 131086](fault_codes/displacement_sensors.md)

## [Inclinometer (0x00030000 mask, range 196609 - 196616)](fault_codes/inclinometer.md)

## [Interlock (0x00040000 mask, range 262145 - 262156](fault_codes/interlock.md)

## ForceControllerSafetyLimit - 0x00050001 (327681)

### Severity

Critical Force Fault - Actuator forces released to park/safe state.

### Rectification

Inspect net calculated force distribution. Check for physical binding, incorrect weight distribution matrices, or erroneous offset inputs.

### Telemetry

MTM1M3\_logevent\_forceControllerFault | safetyLimit

### Setting override

ForceControllerSettings | FaultOnSafetyLimit

## ForceControllerXMomentLimit - 0x00050002 (327682)

### Severity

Critical Force Fault.

### Rectification

Calculated X-axis moment exceeds safety threshold. Check elevation angle inputs and active optic force offset commands.

### Telemetry

MTM1M3\_logevent\_forceControllerFault | xMomentLimit

### Setting override

ForceControllerSettings | FaultOnXMomentLimit

## ForceControllerYMomentLimit - 0x00050003 (327683)

### Severity

Critical Force Fault.

### Rectification

Calculated Y-axis moment exceeds safety threshold. Inspect elevation orientation inputs and side-support actuator force balances.

### Telemetry

MTM1M3\_logevent\_forceControllerFault | yMomentLimit

### Setting override

ForceControllerSettings | FaultOnYMomentLimit

## ForceControllerZMomentLimit - 0x00050004 (327684)

### Severity

Critical Force Fault.

### Rectification

Calculated Z-axis (torsional) moment exceeds safety limit. Verify active optical surface correction values and balance matrix calculations.

### Telemetry

MTM1M3\_logevent\_forceControllerFault | zMomentLimit

### Setting override

ForceControllerSettings | FaultOnZMomentLimit

## ForceControllerNearNeighborCheck - 0x00050005 (327685)

### Severity

Critical Force Fault - Local mirror glass stress prevention.

### Rectification

Relative force differential between adjacent force actuators exceeds limit. Check neighboring load cell readings and calibration factors.

### Telemetry

MTM1M3\_logevent\_forceControllerFault | nearNeighborCheck

### Setting override

ForceControllerSettings | FaultOnNearNeighborCheck

## ForceControllerMagnitudeLimit - 0x00050006 (327686)

### Severity

Critical Force Fault.

### Rectification

Vector sum magnitude of total force on single actuator cell exceeds safe operating threshold. Lower commanded optic offset forces.

### Telemetry

MTM1M3\_logevent\_forceControllerFault | magnitudeLimit

### Setting override

ForceControllerSettings | FaultOnMagnitudeLimit

## ForceControllerFarNeighborCheck - 0x00050007 (327687)

### Severity

Critical Force Fault - Glass figure safety check.

### Rectification

Gradient check across extended actuator zones failed. Inspect spatial force demand profiles for sharp discontinuities or bad input matrices.

### Telemetry

MTM1M3\_logevent\_forceControllerFault | farNeighborCheck

### Setting override

ForceControllerSettings | FaultOnFarNeighborCheck

## ForceControllerElevationForceClipping - 0x00050008 (327688)

### Severity

Warning / Force Limit Clipping.

### Rectification

Elevation-dependent force component clipped to max limit threshold. Verify inclinometer readings and elevation force lookup table settings.

### Telemetry

MTM1M3\_logevent\_forceControllerWarning | elevationForceClipping

### Setting override

ForceControllerSettings | FaultOnElevationForceClipping

## ForceControllerAzimuthForceClipping - 0x00050009 (327689)

### Severity

Warning / Force Limit Clipping.

### Rectification

Azimuth acceleration/wind balance demand clipped. Verify azimuth angle input streams and coordinate frame configuration.

### Telemetry

MTM1M3\_logevent\_forceControllerWarning | azimuthForceClipping

### Setting override

ForceControllerSettings | FaultOnAzimuthForceClipping

## ForceControllerThermalForceClipping - 0x0005000A (327690)

### Severity

Warning / Force Limit Clipping.

### Rectification

Thermal compensation force vector clipped. Check thermal sensor telemetry across cell structure for invalid high temperature spikes.

### Telemetry

MTM1M3\_logevent\_forceControllerWarning | thermalForceClipping

### Setting override

ForceControllerSettings | FaultOnThermalForceClipping

## ForceControllerBalanceForceClipping - 0x0005000B (327691)

### Severity

Warning / Force Limit Clipping.

### Rectification

Calculated static balancing forces exceeded envelope limit. Recalibrate mirror center of gravity matrix settings.

### Telemetry

MTM1M3\_logevent\_forceControllerWarning | balanceForceClipping

### Setting override

ForceControllerSettings | FaultOnBalanceForceClipping

## ForceControllerAccelerationForceClipping - 0x0005000C (327692)

### Severity

Warning / Force Limit Clipping.

### Rectification

Dynamic acceleration compensation force demands clipped. Verify mount acceleration telemetry inputs and dynamic force scaling bounds.

### Telemetry

MTM1M3\_logevent\_forceControllerWarning | accelerationForceClipping

### Setting override

ForceControllerSettings | FaultOnAccelerationForceClipping

## ForceControllerActiveOpticNetForceCheck - 0x0005000D (327693)

### Severity

Critical Force Fault.

### Rectification

Net force contribution from active optics bending modes is non-zero (violates force balance equilibrium). Re-normalize active optics command vector.

### Telemetry

MTM1M3\_logevent\_forceControllerFault | activeOpticNetForceCheck

### Setting override

ForceControllerSettings | FaultOnActiveOpticNetForceCheck

## ForceControllerActiveOpticForceClipping - 0x0005000E (327694)

### Severity

Warning / Force Limit Clipping.

### Rectification

Bending mode correction force truncated at maximum actuator limits. Reduce magnitude of requested wavefront correction Zernike coefficients.

### Telemetry

MTM1M3\_logevent\_forceControllerWarning | activeOpticForceClipping

### Setting override

ForceControllerSettings | FaultOnActiveOpticForceClipping

## ForceControllerStaticForceClipping - 0x0005000F (327695)

### Severity

Warning / Force Limit Clipping.

### Rectification

Baseline deadweight compensation force clipped for target actuator. Verify deadweight table parameters in `StaticForceTable.json`.

### Telemetry

MTM1M3\_logevent\_forceControllerWarning | staticForceClipping

### Setting override

ForceControllerSettings | FaultOnStaticForceClipping

## ForceControllerOffsetForceClipping - 0x00050010 (327696)

### Severity

Warning / Force Limit Clipping.

### Rectification

User/operator manual force offset command clipped. Lower manual force delta inputs to within software allowable bands.

### Telemetry

MTM1M3\_logevent\_forceControllerWarning | offsetForceClipping

### Setting override

ForceControllerSettings | FaultOnOffsetForceClipping

## ForceControllerVelocityForceClipping - 0x00050011 (327697)

### Severity

Warning / Force Limit Clipping.

### Rectification

Damping/velocity force compensation truncated. Verify velocity feedback signals from mount/hardpoints.

### Telemetry

MTM1M3\_logevent\_forceControllerWarning | velocityForceClipping

### Setting override

ForceControllerSettings | FaultOnVelocityForceClipping

## ForceControllerForceClipping - 0x00050012 (327698)

### Severity

Warning / Force Limit Clipping.

### Rectification

Final output demand force clipped by master actuator safety bounds. Inspect combined setpoint profile for excessive total load.

### Telemetry

MTM1M3\_logevent\_forceControllerWarning | forceClipping

### Setting override

ForceControllerSettings | FaultOnForceClipping

## ForceControllerMeasuredXForceLimit - 0x00050013 (327699)

### Severity

Critical Force Fault.

### Rectification

Sum of load cell feedback along X-axis exceeds safety threshold. Inspect hardpoints and lateral supports for mechanical bind or over-tension.

### Telemetry

MTM1M3\_logevent\_forceControllerFault | measuredXForceLimit

### Setting override

ForceControllerSettings | FaultOnMeasuredXForceLimit

## ForceControllerMeasuredYForceLimit - 0x00050014 (327700)

### Severity

Critical Force Fault.

### Rectification

Sum of load cell feedback along Y-axis exceeds safety threshold. Inspect lateral link forces and mirror cell elevation orientation.

### Telemetry

MTM1M3\_logevent\_forceControllerFault | measuredYForceLimit

### Setting override

ForceControllerSettings | FaultOnMeasuredYForceLimit

## ForceControllerMeasuredZForceLimit - 0x00050015 (327701)

### Severity

Critical Force Fault.

### Rectification

Sum of axial load cell readings exceeds safe mirror weight range. Immediately verify pneumatic regulator pressure and axial hardpoint load cells.

### Telemetry

MTM1M3\_logevent\_forceControllerFault | measuredZForceLimit

### Setting override

ForceControllerSettings | FaultOnMeasuredZForceLimit

## CellLightSensorMismatch - 0x00060002 (393218)

### Severity

Operational Warning / Environmental Fault.

### Rectification

Check cell light level status sensors. Mismatch indicates light leak into mirror cell enclosure or sensor failure.

### Telemetry

MTM1M3\_logevent\_cellLightWarning | sensorMismatch

### Setting override

CellLightSettings | FaultOnSensorMismatch

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
