# M1M3 failure codes

Unless noted, setting override are in SafetyControllerSettings section of the init file. Faults codes

## No Fault

Obviously meaning no fault. Used to reset previous error codes.

## AirControllerCommandOutputMismatch - 0x00010001 (65537)

Occurs when commanded value to open the master air valve doesn't match sensed
command state. Most likely indicates either problem in the valve or wiring.
Overriding the problem shall be futile - the mirror cannot be operated without
air.

### Severity

Most likely air valve will remain closed or cannot be closed. Mirror shall not
be operated with this failure.

### Rectification

Check master air valve signals and wiring. Check air valve - does it really opens and
closes as commanded? You can use pressure inside the system (measured on
hardpoints) to confirm valve true state.

### Telemetry

MTM1M3\_logevent\_airSupplyStatus - airCommandedOn, airCommandOutputOn

### Setting override

AirControllerSettings | FaultOnCommandOutputMismatch

## AirControllerCommandSensorMismatch - 0x00010002 (65538)

Occurs when the master air valve sensor value doesn't match expected
(commanded) air valve state. Air valve needs about 30 seconds to change state
(open or close). There is a sensor attached to the valve, which is logical 1
when air valve is closed.

### Rectification

Check wires signalling open valve. Check the air valve.

### Telemetry

MTM1M3\_logevent\_airSupplyStatus - airCommandedOn, airValveOpened, airValveClosed

### Setting override

AirControllerSettings | FaultOnCommandSensorMismatch

## DisplacementSensorReportsInvalidCommand - 0x00020001 (131073)

Internal to displacement sensor logic (DL-RS1A controller). Per DL-RS1A, "Make
sure that the external device has sent a command listed in "Communication
commands" (page 11). As the command is always to read all data (FPGA doesn't
know any other command), occurrence of this error would either point to some
strange issue in the cRIO, FPGA software (bitfile) of DL-RS1A controller.

### Severity

IMS is optional - can be disabled and ignored. IMS is not needed for mirror
operation.

### Rectification

Check DL-RS1A communication unit, changes to CSC and FPGA bitfile.

### Telemetry

MTM1M3_logevent_displacementSensorWarning | sensorReportsInvalidCommand

### Setting override

DisplacementSettings | FaultOnSensorReportsInvalidCommand

## DisplacementSensorReportsCommunicationTimeoutError - 0x00020002 (131074)

Reported by the DL-R51A unit, suggest problem of the controller communicating
with the sensors. Communication could not be established between the DL and the
amplifier.

### Severity

IMS is optional - can be disabled and ignored. IMS is not needed for mirror
operation.

### Rectification

Check DL-RS1A communication unit serial port wiring, power and cRIO module
reading out the serial port (NI-9870 unit, Slot 8, port 1 of support system
cRIO). Per RL-RS1A, check if the GT2-100 is not in the initial reset process or
reflecting the valid ID setting.

### Telemetry

MTM1M3_logevent_displacementSensorWarning | sensorReportsCommunicationTimeoutError

### Setting override

DisplacementSettings | FaultOnSensorReportsCommunicationTimeoutError

## DisplacementSensorReportsDataLengthError  - 0x00020003 (131075)

Data with the correct length was not received. Not expected, as the CSC doesn't
send any data to IMS unit - it sends only requests to retrieve data.

### Severity

IMS is optional - can be disabled and ignored. IMS is not needed for mirror
operation.

### Rectification

Check unit response. If IMS data are still flowing in and doesn't show any
problem, most likely this was a random error. If it re-occurs, try to replace
IMS serial - sensor unit (DL-RS1A).

### Telemetry

MTM1M3_logevent_displacementSensorWarning | sensorReportsDataLenghtError

### Setting override

DisplacementSettings | FaultOnSensorReportsDataLengthError

## DisplacementSensorReportsNumberOfParametersError - 0x00020004 (131076)

The correct number of parameters for the command was not received. Not expected, as the CSC doesn't
send any data to IMS unit - it sends only requests to retrieve data.

### Severit 

IMS is optional - can be disabled and ignored. IMS is not needed for mirror operation.

### Rectification

Check unit response. If IMS data are still flowing in and doesn't show any
problem, most likely this was a random error. If it re-occurs, try to replace
IMS serial - sensor unit (DL-RS1A).

### Telemetry

MTM1M3_logevent_displacementSensorWarning | sensorReportsNumberOfParametersError

### Setting override

DisplacementSettings | FaultOnSensorReportsNumberOfParametersError

## DisplacementSensorReportsParameterError - 0x00020005 (131077)

* A parameter exceeds its ranga of value.
* The external device is trying to write a data type that cannot be written.
* The external device is trying to read a data type that cannot be read.
* The data format is incorrect

Not expected, as the CSC doesn't send any data to IMS unit - it sends only
requests to retrieve data.

### Severity

IMS is optional - can be disabled and ignored. IMS is not needed for mirror operation.

### Rectification

Check unit response. If IMS data are still flowing in and doesn't show any
problem, most likely this was a random error. If it re-occurs, try to replace
IMS serial - sensor unit (DL-RS1A).

### Telemetry

MTM1M3_logevent_displacementSensorWarning | sensorReportsParameterError

### Setting override

DisplacementSettings | FaultOnSensorReportsParameterError

## DisplacementSensorReportsCommunicationError - 0x00020006 (131078)

An error was detected with RS-232C communication.

### Severity

IMS is optional - can be disabled and ignored. IMS is not needed for mirror operation.

### Rectification

Make sure that DL-RS1A and the external device have the same communication
settings configured. For information on configuring DL-RS1A, refer to page 2 of
the DL-RS1A user manual (available on Wiki Datasheets page).

### Telemetry

MTM1M3_logevent_displacementSensorWarning | sensorReportsCommunicationError

### Setting override

DisplacementSettings | FaultOnSensorReportsCommunicationError

## DisplacementSensorReportsIDNumberError - 0x00020007 (131079)

The ID number specified with the command is incorrect. Not expected, as the CSC
doesn't send any data to IMS unit - it sends only requests to retrieve data.

### Severity

IMS is optional - can be disabled and ignored. IMS is not needed for mirror operation.

### Rectification

Check unit response. If IMS data are still flowing in and doesn't show any
problem, most likely this was a random error. If it re-occurs, try to replace
IMS serial - sensor unit (DL-RS1A).

### Telemetry

MTM1M3_logevent_displacementSensorWarning | sensorReportsIDNumberError

### Setting override

DisplacementSettings | FaultOnSensorReportsIDNumberError

## DisplacementSensorReportsExpansionLineError - 0x00020008 (131080)

The communication could not be established due to a problem with an expansion line.

### Severity

IMS is optional - can be disabled and ignored. IMS is not needed for mirror operation.

### Rectification

Check that each of the sensor amplifiers and DL-RS1A are securely and properly
connected by referring to "Connecting the Unit to Sensor Amplifiers" (page 3i
of DL-RS1A User manual - in Wiki Datasheets).

Make sure that sensor amplifiers that are supported by DL-RS1A are connected
(refer to page 4 of the DL-RS1A User manual).Check if the GT2-100 is not in the
initial reset process or reflecting the valid ID setting

### Telemetry

MTM1M3_logevent_displacementSensorWarning | sensorReportsExpansionLineError

### Setting override

DisplacementSettings | FaultOnSensorReportsExpansionLineError

## DisplacementSensorReportsWriteControlError - 0x00020009 (131081)

DL-RS1A is not writable. Not expected, as the CSC doesn't send any data to IMS
unit - it sends only requests to retrieve data.

### Severity

IMS is optional - can be disabled and ignored. IMS is not needed for mirror operation.

### Rectification

Check unit response. If IMS data are still flowing in and doesn't show any
problem, most likely this was a random error. If it re-occurs, try to replace
IMS serial - sensor unit (DL-RS1A).

### Telemetry

MTM1M3_logevent_displacementSensorWarning | sensorReportsWriteControlError

### Setting override

DisplacementSettings | FaultOnSensorReportsWriteControlError

## DisplacementResponseTimeoutError - 0x0002000A (131082)

Indicates timeout in communication between cRIO NI-9870 serial module and DL-RS1A IMS unit.

### Severity

IMS is optional - can be disabled and ignored. IMS is not needed for mirror operation.

### Rectification

Confirm communication port availability on NI-9870 serial module. Check the
DL-RS1A module is powered and properly wired. Try to replace DL-RS1A and NI-9870 serial module.

### Telemetry

MTM1M3_logevent_displacementSensorWarning | responseTimeoutError

### Setting override

DisplacementSettings | FaultOnResponseTimeoutError

## DisplacementInvalidLength - 0x0002000B (131083)

Triggered on invalid length of reply from the DL-RS1A. Most likely cause would
be some DL-RS1A problems. This shall be similar to timeout problem (error
0x2000A).

### Severity

IMS is optional - can be disabled and ignored. IMS is not needed for mirror operation.

### Rectification

Check unit response. If IMS data are still flowing in and doesn't show any
problem, most likely this was a random error. If it re-occurs, try to replace
IMS serial - sensor unit (DL-RS1A).

### Telemetry

MTM1M3_logevent_displacementSensorWarning | invalidLength

### Setting override

DisplacementSettings | FaultOnInvalidLength

## DisplacementUnknownCommand - 0x0002000C (131084)

Wrong reply from the DL-RS1A unit.

### Severity

IMS is optional - can be disabled and ignored. IMS is not needed for mirror operation.

### Rectification

Check unit response. If IMS data are still flowing in and doesn't show any
problem, most likely this was a random error. If it re-occurs, try to replace
IMS serial - sensor unit (DL-RS1A).

### Telemetry

MTM1M3_logevent_displacementSensorWarning | unknownCommand

### Setting override

DisplacementSettings | FaultOnUnknownCommand

## DisplacementUnknownProblem - 0x0002000D (131085)

Most likely problem in DL-RS1A communication.

### Severity

IMS is optional - can be disabled and ignored. IMS is not needed for mirror operation.

### Rectification

Check unit response. If IMS data are still flowing in and doesn't show any
problem, most likely this was a random error. If it re-occurs, try to replace
IMS serial - sensor unit (DL-RS1A).

### Telemetry

MTM1M3_logevent_displacementSensorWarning | unknownProblem

### Setting override

DisplacementSettings | FaultOnUnknownProblem

## DisplacementInvalidResponse - 0x0002000E (131086)

Wrong reply from the DL-RS1A unit.

### Severity

IMS is optional - can be disabled and ignored. IMS is not needed for mirror operation.

### Rectification

Check unit response. If IMS data are still flowing in and doesn't show any
problem, most likely this was a random error. If it re-occurs, try to replace
IMS serial - sensor unit (DL-RS1A).

### Telemetry

MTM1M3_logevent_displacementSensorWarning | invalidResponse

### Setting override

DisplacementSettings | FaultOnInvalidResponse

## InclinometerResponseTimeout - 0x00030001 (196609)

### Severity

Critical warning during movement and positioning operations.

### Rectification

Check RS-485 Modbus serial line to the inclinometer sensor. Confirm Modbus ID and baud rate settings on the cRIO serial port module.

### Telemetry

MTM1M3_logevent_inclinometerWarning | responseTimeout

### Setting override

InclinometerSettings | FaultOnResponseTimeout

## InclinometerInvalidCRC - 0x00030002 (196610)

### Severity

Warning - data frame dropped.

### Rectification

Inspect line termination resistors (120 Ohm) on the Modbus network. Check cable shielding and grounding near high-power force actuators.

### Telemetry

MTM1M3_logevent_inclinometerWarning | invalidCRC

### Setting override

InclinometerSettings | FaultOnInvalidCRC

## InclinometerUnknownAddress - 0x00030003 (196611)

### Severity

Configuration Error.

### Rectification

Verify target Modbus slave address in `InclinometerSettings.json` matches the DIP-switch / internal address of the inclinometer unit.

### Telemetry

MTM1M3_logevent_inclinometerWarning | unknownAddress

### Setting override

InclinometerSettings | FaultOnUnknownAddress

## InclinometerUnknownFunction - 0x00030004 (196612)

### Severity

Configuration Warning.

### Rectification

Verify Modbus function code sent by cRIO (e.g., Read Holding Registers 0x03) is supported by the inclinometer firmware.

### Telemetry

MTM1M3_logevent_inclinometerWarning | unknownFunction

### Setting override

InclinometerSettings | FaultOnUnknownFunction

## InclinometerInvalidLength - 0x00030005 (196613)

### Severity

Warning - frame dropped.

### Rectification

Check frame parsing logic in `Inclinometer` driver component and verify standard Modbus RTU payload lengths.

### Telemetry

MTM1M3_logevent_inclinometerWarning | invalidLength

### Setting override

InclinometerSettings | FaultOnInvalidLength

## InclinometerSensorReportsIllegalDataAddress - 0x00030006 (196614)

### Severity

Configuration Error.

### Rectification

Check register offset addresses mapped in configuration files against the vendor memory map for the inclinometer model.

### Telemetry

MTM1M3_logevent_inclinometerWarning | sensorReportsIllegalDataAddress

### Setting override

InclinometerSettings | FaultOnSensorReportsIllegalDataAddress

## InclinometerSensorReportsIllegalFunction - 0x00030007 (196615)

### Severity

Configuration Error.

### Rectification

Ensure request types match read-only vs read/write permissions on the inclinometer registers.

### Telemetry

MTM1M3_logevent_inclinometerWarning | sensorReportsIllegalFunction

### Setting override

InclinometerSettings | FaultOnSensorReportsIllegalFunction

## InclinometerUnknownProblem - 0x00030008 (196616)

### Severity

General Warning.

### Rectification

Power cycle the inclinometer power bus and inspect health registers via direct Modbus diagnostic query.

### Telemetry

MTM1M3_logevent_inclinometerWarning | unknownProblem

### Setting override

InclinometerSettings | FaultOnUnknownProblem

## InterlockHeartbeatStateOutputMismatch - 0x00040001 (262145)

### Severity

Critical Safety Fault - Prevents or halts active mirror support operation.

### Rectification

Inspect safety FPGA digital IO line for heartbeat toggle. Confirm handshake timing between Safety Controller and Main Controller cRIO.

### Telemetry

MTM1M3_logevent_interlockSystemFault | heartbeatStateOutputMismatch

### Setting override

SafetySettings | FaultOnHeartbeatStateOutputMismatch

## InterlockCriticalFaultStateOutputMismatch - 0x00040002 (262146)

### Severity

Critical Safety Fault.

### Rectification

Check active fault matrix on the Global Interlock System (GIS) and hardwired safety loop circuits. Resolve underlying critical trip before resetting interlock.

### Telemetry

MTM1M3_logevent_interlockSystemFault | criticalFaultStateOutputMismatch

### Setting override

SafetySettings | FaultOnCriticalFaultStateOutputMismatch

## InterlockMirrorLoweringRaisingStateOutputMismatch - 0x00040003 (262147)

### Severity

Critical Safety Fault.

### Rectification

Verify state machine transitions for mirror raise/lower sequences. Check digital feedback signals from hardpoints and static support mechanisms.

### Telemetry

MTM1M3_logevent_interlockSystemFault | mirrorLoweringRaisingStateOutputMismatch

### Setting override

SafetySettings | FaultOnMirrorLoweringRaisingStateOutputMismatch

## InterlockMirrorParkedStateOutputMismatch - 0x00040004 (262148)

### Severity

Critical Safety Fault.

### Rectification

Verify physical limit switch outputs and displacement sensors confirm mirror is securely resting on static supports (parked state).

### Telemetry

MTM1M3_logevent_interlockSystemFault | mirrorParkedStateOutputMismatch

### Setting override

SafetySettings | FaultOnMirrorParkedStateOutputMismatch

## InterlockPowerNetworksOff - 0x00040005 (262149)

### Severity

Operational Interlock Fault.

### Rectification

Inspect AC/DC power supply breakers and relay outputs for actuator power buses A, B, C, and D. Ensure main power relays are commanded ON.

### Telemetry

MTM1M3_logevent_interlockSystemFault | powerNetworksOff

### Setting override

SafetySettings | FaultOnPowerNetworksOff

## InterlockThermalEquipmentOff - 0x00040006 (262150)

### Severity

Operational Interlock Warning / Fault.

### Rectification

Verify thermal control system power and interlock contactors are engaged. Inspect fan and glycol cooling loop status inputs.

### Telemetry

MTM1M3_logevent_interlockSystemFault | thermalEquipmentOff

### Setting override

SafetySettings | FaultOnThermalEquipmentOff

## InterlockLaserTrackerOff - 0x00040007 (262151)

### Severity

Warning / System Interlock.

### Rectification

Verify metrology laser tracker power status and connection to external safety system interlock loop.

### Telemetry

MTM1M3_logevent_interlockSystemFault | laserTrackerOff

### Setting override

SafetySettings | FaultOnLaserTrackerOff

## InterlockAirSupplyOff - 0x00040008 (262152)

### Severity

Critical Interlock Fault - Pneumatic force actuators require pressurized air.

### Rectification

Check main pneumatic supply pressure gauge and digital pressure switches. Confirm supply valve is open and operating above minimum bar threshold.

### Telemetry

MTM1M3_logevent_interlockSystemFault | airSupplyOff

### Setting override

SafetySettings | FaultOnAirSupplyOff

## InterlockGISEarthquake - 0x00040009 (262153)

### Severity

Emergency Safety Trip.

### Rectification

Seismic trigger detected by Global Interlock System. Verify structural integrity of mirror, cell, and supports prior to resetting seismic trip state.

### Telemetry

MTM1M3_logevent_interlockSystemFault | gisEarthquake

### Setting override

SafetySettings | FaultOnGISEarthquake

## InterlockGISEStop - 0x0004000A (262154)

### Severity

Emergency Safety Trip - Immediate force release to safe state.

### Rectification

Locate and clear active Emergency Stop button press on GIS console or local M1M3 cell interface panels. Reset E-stop relay circuit.

### Telemetry

MTM1M3_logevent_interlockSystemFault | gisEStop

### Setting override

SafetySettings | FaultOnGISEStop

## InterlockTMAMotionStop - 0x0004000B (262155)

### Severity

Operational Interlock.

### Rectification

Telescope Mount Assembly (TMA) commanded motion stop. Wait for TMA safety system clear signal before re-engaging active optics controller forces.

### Telemetry

MTM1M3_logevent_interlockSystemFault | tmaMotionStop

### Setting override

SafetySettings | FaultOnTMAMotionStop

## InterlockGISHeartbeatLost - 0x0004000C (262156)

### Severity

Critical Interlock Fault.

### Rectification

Check network line and hardwired pulse train connecting M1M3 controller to Global Interlock System. Confirm GIS controller power status.

### Telemetry

MTM1M3_logevent_interlockSystemFault | gisHeartbeatLost

### Setting override

SafetySettings | FaultOnGISHeartbeatLost

## ForceControllerSafetyLimit - 0x00050001 (327681)

### Severity

Critical Force Fault - Actuator forces released to park/safe state.

### Rectification

Inspect net calculated force distribution. Check for physical binding, incorrect weight distribution matrices, or erroneous offset inputs.

### Telemetry

MTM1M3_logevent_forceControllerFault | safetyLimit

### Setting override

ForceControllerSettings | FaultOnSafetyLimit

## ForceControllerXMomentLimit - 0x00050002 (327682)

### Severity

Critical Force Fault.

### Rectification

Calculated X-axis moment exceeds safety threshold. Check elevation angle inputs and active optic force offset commands.

### Telemetry

MTM1M3_logevent_forceControllerFault | xMomentLimit

### Setting override

ForceControllerSettings | FaultOnXMomentLimit

## ForceControllerYMomentLimit - 0x00050003 (327683)

### Severity

Critical Force Fault.

### Rectification

Calculated Y-axis moment exceeds safety threshold. Inspect elevation orientation inputs and side-support actuator force balances.

### Telemetry

MTM1M3_logevent_forceControllerFault | yMomentLimit

### Setting override

ForceControllerSettings | FaultOnYMomentLimit

## ForceControllerZMomentLimit - 0x00050004 (327684)

### Severity

Critical Force Fault.

### Rectification

Calculated Z-axis (torsional) moment exceeds safety limit. Verify active optical surface correction values and balance matrix calculations.

### Telemetry

MTM1M3_logevent_forceControllerFault | zMomentLimit

### Setting override

ForceControllerSettings | FaultOnZMomentLimit

## ForceControllerNearNeighborCheck - 0x00050005 (327685)

### Severity

Critical Force Fault - Local mirror glass stress prevention.

### Rectification

Relative force differential between adjacent force actuators exceeds limit. Check neighboring load cell readings and calibration factors.

### Telemetry

MTM1M3_logevent_forceControllerFault | nearNeighborCheck

### Setting override

ForceControllerSettings | FaultOnNearNeighborCheck

## ForceControllerMagnitudeLimit - 0x00050006 (327686)

### Severity

Critical Force Fault.

### Rectification

Vector sum magnitude of total force on single actuator cell exceeds safe operating threshold. Lower commanded optic offset forces.

### Telemetry

MTM1M3_logevent_forceControllerFault | magnitudeLimit

### Setting override

ForceControllerSettings | FaultOnMagnitudeLimit

## ForceControllerFarNeighborCheck - 0x00050007 (327687)

### Severity

Critical Force Fault - Glass figure safety check.

### Rectification

Gradient check across extended actuator zones failed. Inspect spatial force demand profiles for sharp discontinuities or bad input matrices.

### Telemetry

MTM1M3_logevent_forceControllerFault | farNeighborCheck

### Setting override

ForceControllerSettings | FaultOnFarNeighborCheck

## ForceControllerElevationForceClipping - 0x00050008 (327688)

### Severity

Warning / Force Limit Clipping.

### Rectification

Elevation-dependent force component clipped to max limit threshold. Verify inclinometer readings and elevation force lookup table settings.

### Telemetry

MTM1M3_logevent_forceControllerWarning | elevationForceClipping

### Setting override

ForceControllerSettings | FaultOnElevationForceClipping

## ForceControllerAzimuthForceClipping - 0x00050009 (327689)

### Severity

Warning / Force Limit Clipping.

### Rectification

Azimuth acceleration/wind balance demand clipped. Verify azimuth angle input streams and coordinate frame configuration.

### Telemetry

MTM1M3_logevent_forceControllerWarning | azimuthForceClipping

### Setting override

ForceControllerSettings | FaultOnAzimuthForceClipping

## ForceControllerThermalForceClipping - 0x0005000A (327690)

### Severity

Warning / Force Limit Clipping.

### Rectification

Thermal compensation force vector clipped. Check thermal sensor telemetry across cell structure for invalid high temperature spikes.

### Telemetry

MTM1M3_logevent_forceControllerWarning | thermalForceClipping

### Setting override

ForceControllerSettings | FaultOnThermalForceClipping

## ForceControllerBalanceForceClipping - 0x0005000B (327691)

### Severity

Warning / Force Limit Clipping.

### Rectification

Calculated static balancing forces exceeded envelope limit. Recalibrate mirror center of gravity matrix settings.

### Telemetry

MTM1M3_logevent_forceControllerWarning | balanceForceClipping

### Setting override

ForceControllerSettings | FaultOnBalanceForceClipping

## ForceControllerAccelerationForceClipping - 0x0005000C (327692)

### Severity

Warning / Force Limit Clipping.

### Rectification

Dynamic acceleration compensation force demands clipped. Verify mount acceleration telemetry inputs and dynamic force scaling bounds.

### Telemetry

MTM1M3_logevent_forceControllerWarning | accelerationForceClipping

### Setting override

ForceControllerSettings | FaultOnAccelerationForceClipping

## ForceControllerActiveOpticNetForceCheck - 0x0005000D (327693)

### Severity

Critical Force Fault.

### Rectification

Net force contribution from active optics bending modes is non-zero (violates force balance equilibrium). Re-normalize active optics command vector.

### Telemetry

MTM1M3_logevent_forceControllerFault | activeOpticNetForceCheck

### Setting override

ForceControllerSettings | FaultOnActiveOpticNetForceCheck

## ForceControllerActiveOpticForceClipping - 0x0005000E (327694)

### Severity

Warning / Force Limit Clipping.

### Rectification

Bending mode correction force truncated at maximum actuator limits. Reduce magnitude of requested wavefront correction Zernike coefficients.

### Telemetry

MTM1M3_logevent_forceControllerWarning | activeOpticForceClipping

### Setting override

ForceControllerSettings | FaultOnActiveOpticForceClipping

## ForceControllerStaticForceClipping - 0x0005000F (327695)

### Severity

Warning / Force Limit Clipping.

### Rectification

Baseline deadweight compensation force clipped for target actuator. Verify deadweight table parameters in `StaticForceTable.json`.

### Telemetry

MTM1M3_logevent_forceControllerWarning | staticForceClipping

### Setting override

ForceControllerSettings | FaultOnStaticForceClipping

## ForceControllerOffsetForceClipping - 0x00050010 (327696)

### Severity

Warning / Force Limit Clipping.

### Rectification

User/operator manual force offset command clipped. Lower manual force delta inputs to within software allowable bands.

### Telemetry

MTM1M3_logevent_forceControllerWarning | offsetForceClipping

### Setting override

ForceControllerSettings | FaultOnOffsetForceClipping

## ForceControllerVelocityForceClipping - 0x00050011 (327697)

### Severity

Warning / Force Limit Clipping.

### Rectification

Damping/velocity force compensation truncated. Verify velocity feedback signals from mount/hardpoints.

### Telemetry

MTM1M3_logevent_forceControllerWarning | velocityForceClipping

### Setting override

ForceControllerSettings | FaultOnVelocityForceClipping

## ForceControllerForceClipping - 0x00050012 (327698)

### Severity

Warning / Force Limit Clipping.

### Rectification

Final output demand force clipped by master actuator safety bounds. Inspect combined setpoint profile for excessive total load.

### Telemetry

MTM1M3_logevent_forceControllerWarning | forceClipping

### Setting override

ForceControllerSettings | FaultOnForceClipping

## ForceControllerMeasuredXForceLimit - 0x00050013 (327699)

### Severity

Critical Force Fault.

### Rectification

Sum of load cell feedback along X-axis exceeds safety threshold. Inspect hardpoints and lateral supports for mechanical bind or over-tension.

### Telemetry

MTM1M3_logevent_forceControllerFault | measuredXForceLimit

### Setting override

ForceControllerSettings | FaultOnMeasuredXForceLimit

## ForceControllerMeasuredYForceLimit - 0x00050014 (327700)

### Severity

Critical Force Fault.

### Rectification

Sum of load cell feedback along Y-axis exceeds safety threshold. Inspect lateral link forces and mirror cell elevation orientation.

### Telemetry

MTM1M3_logevent_forceControllerFault | measuredYForceLimit

### Setting override

ForceControllerSettings | FaultOnMeasuredYForceLimit

## ForceControllerMeasuredZForceLimit - 0x00050015 (327701)

### Severity

Critical Force Fault.

### Rectification

Sum of axial load cell readings exceeds safe mirror weight range. Immediately verify pneumatic regulator pressure and axial hardpoint load cells.

### Telemetry

MTM1M3_logevent_forceControllerFault | measuredZForceLimit

### Setting override

ForceControllerSettings | FaultOnMeasuredZForceLimit

## CellLightSensorMismatch - 0x00060002 (393218)

### Severity

Operational Warning / Environmental Fault.

### Rectification

Check cell light level status sensors. Mismatch indicates light leak into mirror cell enclosure or sensor failure.

### Telemetry

MTM1M3_logevent_cellLightWarning | sensorMismatch

### Setting override

CellLightSettings | FaultOnSensorMismatch

## PowerControllerPowerNetworkAOutputMismatch - 0x00070001 (458753)

### Severity

Hardware Power Fault.

### Rectification

Commanded status for Power Network A does not match feedback contactor relay. Check 24V supply fuses and cRIO DO/DI module wiring for Sub-network A.

### Telemetry

MTM1M3_logevent_powerSupplyFault | powerNetworkAOutputMismatch

### Setting override

PowerControllerSettings | FaultOnPowerNetworkAOutputMismatch

## PowerControllerPowerNetworkBOutputMismatch - 0x00070002 (458754)

### Severity

Hardware Power Fault.

### Rectification

Check contactors, circuit breakers, and digital feedback lines for Power Network B.

### Telemetry

MTM1M3_logevent_powerSupplyFault | powerNetworkBOutputMismatch

### Setting override

PowerControllerSettings | FaultOnPowerNetworkBOutputMismatch

## PowerControllerPowerNetworkCOutputMismatch - 0x00070003 (458755)

### Severity

Hardware Power Fault.

### Rectification

Check contactors, circuit breakers, and digital feedback lines for Power Network C.

### Telemetry

MTM1M3_logevent_powerSupplyFault | powerNetworkCOutputMismatch

### Setting override

PowerControllerSettings | FaultOnPowerNetworkCOutputMismatch

## PowerControllerPowerNetworkDOutputMismatch - 0x00070004 (458756)

### Severity

Hardware Power Fault.

### Rectification

Check contactors, circuit breakers, and digital feedback lines for Power Network D.

### Telemetry

MTM1M3_logevent_powerSupplyFault | powerNetworkDOutputMismatch

### Setting override

PowerControllerSettings | FaultOnPowerNetworkDOutputMismatch

## PowerControllerAuxPowerNetworkAOutputMismatch - 0x00070005 (458757)

### Severity

Hardware Power Fault.

### Rectification

Inspect auxiliary power bus A distribution panel, switching relays, and monitor input lines.

### Telemetry

MTM1M3_logevent_powerSupplyFault | auxPowerNetworkAOutputMismatch

### Setting override

PowerControllerSettings | FaultOnAuxPowerNetworkAOutputMismatch

## PowerControllerAuxPowerNetworkBOutputMismatch - 0x00070006 (458758)

### Severity

Hardware Power Fault.

### Rectification

Inspect auxiliary power bus B distribution panel, switching relays, and monitor input lines.

### Telemetry

MTM1M3_logevent_powerSupplyFault | auxPowerNetworkBOutputMismatch

### Setting override

PowerControllerSettings | FaultOnAuxPowerNetworkBOutputMismatch

## PowerControllerAuxPowerNetworkCOutputMismatch - 0x00070007 (458759)

### Severity

Hardware Power Fault.

### Rectification

Inspect auxiliary power bus C distribution panel, switching relays, and monitor input lines.

### Telemetry

MTM1M3_logevent_powerSupplyFault | auxPowerNetworkCOutputMismatch

### Setting override

PowerControllerSettings | FaultOnAuxPowerNetworkCOutputMismatch

## PowerControllerAuxPowerNetworkDOutputMismatch - 0x00070008 (458760)

### Severity

Hardware Power Fault.

### Rectification

Inspect auxiliary power bus D distribution panel, switching relays, and monitor input lines.

### Telemetry

MTM1M3_logevent_powerSupplyFault | auxPowerNetworkDOutputMismatch

### Setting override

PowerControllerSettings | FaultOnAuxPowerNetworkDOutputMismatch

## RaiseOperationTimeout - 0x00080001 (524289)

### Severity

Operational Fault - Raise operation aborted.

### Rectification

Mirror raise sequence took longer than maximum configured timeout. Check pneumatic system pressure build rate and hardpoint motion progress.

### Telemetry

MTM1M3_logevent_supportOperationFault | raiseOperationTimeout

### Setting override

SupportOperationSettings | FaultOnRaiseOperationTimeout

## LowerOperationTimeout - 0x00080002 (524290)

### Severity

Operational Fault - Lower operation aborted.

### Rectification

Mirror lower sequence failed to reach static parked state within time limit. Check pneumatic bleed rates and hardpoint retraction status.

### Telemetry

MTM1M3_logevent_supportOperationFault | lowerOperationTimeout

### Setting override

SupportOperationSettings | FaultOnLowerOperationTimeout

## ILCCommunicationTimeout - 0x00080003 (524291)

### Severity

Critical Communication Fault - Actuator control disrupted.

### Rectification

Check subnet communication loops to Individual Loop Controllers (ILC). Verify power and bus connections to subnet Modbus/RS485 master nodes.

### Telemetry

MTM1M3_logevent_ilcWarning | communicationTimeout

### Setting override

ILCSettings | FaultOnCommunicationTimeout

## ModbusIRQTimeout - 0x00080004 (524292)

### Severity

System Timing / Communication Fault.

### Rectification

Interrupt processing timeout on cRIO Modbus interface module. Check CPU utilization on cRIO and inspect serial traffic for dropped interrupt signals.

### Telemetry

MTM1M3_logevent_ilcWarning | modbusIRQTimeout

### Setting override

ILCSettings | FaultOnModbusIRQTimeout

## ForceActuatorFollowingErrorCounting - 0x00090001 (589825)

### Severity

Actuator Performance Warning / Fault.

### Rectification

Force actuator output feedback deviates from command setpoint continuously. Inspect specific pneumatic valve response and calibration parameters.

### Telemetry

MTM1M3_logevent_forceActuatorWarning | followingErrorCounting

### Setting override

ForceActuatorSettings | FaultOnFollowingErrorCounting

## ForceActuatorFollowingErrorImmediate - 0x00090002 (589826)

### Severity

Critical Actuator Fault - Immediate shutdown of affected node/subsystem.

### Rectification

Large sudden step error between commanded and measured force on actuator. Check for line pressure drop, disconnected load cell cabling, or failed valve controller.

### Telemetry

MTM1M3_logevent_forceActuatorFault | followingErrorImmediate

### Setting override

ForceActuatorSettings | FaultOnFollowingErrorImmediate

## HardpointActuator - 0x000A0001 (655361)

### Severity

General Hardpoint Fault.

### Rectification

General fault reported on hardpoint positioning mechanism. Check encoder feedback, drive motor power, and internal health registers.

### Telemetry

MTM1M3_logevent_hardpointActuatorFault | hardpointActuator

### Setting override

HardpointSettings | FaultOnHardpointActuator

## HardpointActuatorLoadCellError - 0x000A0002 (655362)

### Severity

Critical Hardpoint Fault.

### Rectification

Load cell reading on target hardpoint is out of valid electrical/physical bounds. Check strain gauge excitation voltage, wiring, and ADC readout module.

### Telemetry

MTM1M3_logevent_hardpointActuatorFault | loadCellError

### Setting override

HardpointSettings | FaultOnLoadCellError

## HardpointActuatorMeasuredForceError - 0x000A0003 (655363)

### Severity

Critical Hardpoint Fault.

### Rectification

Measured force on hardpoint exceeds safe operational envelope during active optics mode. Verify load sharing between force actuators and hardpoints.

### Telemetry

MTM1M3_logevent_hardpointActuatorFault | measuredForceError

### Setting override

HardpointSettings | FaultOnMeasuredForceError

## HardpointHighTension - 0x000A0004 (655364)

### Severity

Critical Safety Fault - Prevents mirror damage from excessive tensile loads.

### Rectification

Hardpoint load cell registered tension force exceeding maximum tension limit. Immediately verify mirror force balance and lower active optical forces.

### Telemetry

MTM1M3_logevent_hardpointActuatorFault | highTension

### Setting override

HardpointSettings | FaultOnHighTension
