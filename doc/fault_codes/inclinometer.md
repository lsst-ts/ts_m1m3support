# Inclinometer (0x00030000 mask)

The M1M3 inclinometer is a key orientation sensor mounted on the mirror cell
structure. Its primary function is to measure the real-time elevation angle of
the M1M3 mirror cell relative to the gravity vector.

As the Telescope Mount Assembly (TMA) rotates in elevation, the direction and
magnitude of gravitational forces acting on the 17-ton glass mirror change
continuously. The inclinometer provides the primary elevation angle input to
the M1M3 Support Controller. The controller uses this angle as parameter to
query a Look-Up Table (LUT) — interpolating via 5th-order splines — to
calculate the exact axial and lateral support forces required from the 156
pneumatic force actuators to support the mirror without deforming its optical
figure.

## Severity

The support system requires verified elevation data for operation. Although TMA
elevation data may serve as an alternative, operational experience demonstrates
that its reliability degrades in boundary TMA states. Flawed elevation data
risks inducing excessive mirror stress and critical glass oscillations,
negatively impacting structural safety.

## Rectification

Switching to TMA Elevation - setting ForceActuatorSettings | UseInclinometer to
False - is highly discouraged. TMA can at times provide inaccurate elevation,
and this can prove fatal for the glass. Much safer would be to move mirror on
static supports to zenith and fix inclinometer problem.

Check inclinometer Modbus address - refer to T7 inclinometer documentation in
Wiki Datasheets. Check RS-485 Modbus serial line to the inclinometer sensor.
Confirm Modbus ID and baud rate settings on the cRIO serial port module. Check
inclinometer power supply. Replace inclinometer and/or NI-9870 module.

If the Displacement / IMS sensors shows communication issues, the most
plausible common element to cause this is the NI-9870 serial module both units
are connected.

Inclinometer is connected to port 2 of the NI-9870 serial module in slot 8 of
support system cRIO.

## Telemetry

MTM1M3\_inclinometerData | inclinometerAngle

## Setting override

Those two settings controls allowable deviation among TMA and inclinometer, and
if inclinometer is used at all. 

UseInclinometer shall be set to False to not use inclinometer at all.

When InclinometerDeviationDeg is equal or less than 0, discrepancy between TMA
elevation and inclinometer angle is ignored. Otherwise, the absolute 

TMASettings | InclinometerDeviationDeg

ForceActuatorSettings | UseInclinometer

<a name="0x00030001"></a>
## InclinometerResponseTimeout - 0x00030001 (196609)

Inclinometer is not sending data. Inclinometer is connected to port 2 of the
slot 8 NI-9870 unit, and uses serial port communication. Any interruption will
cause inclinometer timeout.

### Telemetry

MTM1M3\_logevent\_inclinometerWarning | responseTimeout

### Setting override

InclinometerSettings | FaultOnResponseTimeout

<a name="0x00030002"></a>
## InclinometerInvalidCRC - 0x00030002 (196610)

Invalid CRC in inclinometer Modbus communication. 

### Telemetry

MTM1M3\_logevent\_inclinometerWarning | invalidCRC

### Setting override

InclinometerSettings | FaultOnInvalidCRC

<a name="0x00030003"></a>
## InclinometerUnknownAddress - 0x00030003 (196611)

Inclinometer Modbus reply contains unknown source (reply originator) address.
Inclinometer is assumed to be on original, default 127 address. If its address
shall be changed, the address must be replaced in multiple .vi (LabVIEW FPGA
source files).

Most likely inclinometer T7 unit issue. Very unlikely to ever appear, as the
inclinometer shall not respond to queries send to different address.

### Telemetry

MTM1M3\_logevent\_inclinometerWarning | unknownAddress

### Setting override

InclinometerSettings | FaultOnUnknownAddress

<a name="0x00030004"></a>
## InclinometerUnknownFunction - 0x00030004 (196612)

Inclinometer unit Modbus reply contains wrong function code. Unexpected to
occur, most likely caused by defective inclinometer unit or significant noise
on inclinometer serial line.

### Telemetry

MTM1M3\_logevent\_inclinometerWarning | unknownFunction

### Setting override

InclinometerSettings | FaultOnUnknownFunction

<a name="0x00030005"></a>
## InclinometerInvalidLength - 0x00030005 (196613)

Invalid length of Modbus response from the inclinometer unit.

### Telemetry

MTM1M3\_logevent\_inclinometerWarning | invalidLength

### Setting override

InclinometerSettings | FaultOnInvalidLength

<a name="0x00030006"></a>
## InclinometerSensorReportsIllegalDataAddress - 0x00030006 (196614)

Modbus exception code is equal to 2 (see
https://simplymodbus.ca/learn-exceptions.html for list of Exception Codes).

Very unlikely to occur. If it occurs, it is probably inclinometer problem.

### Telemetry

MTM1M3\_logevent\_inclinometerWarning | sensorReportsIllegalDataAddress

### Setting override

InclinometerSettings | FaultOnSensorReportsIllegalDataAddress

<a name="0x00030007"></a>
## InclinometerSensorReportsIllegalFunction - 0x00030007 (196615)

Modbus exception code is equal to 1 (see
https://simplymodbus.ca/learn-exceptions.html for list of Exception Codes).

Very unlikely to occur. If it occurs, it is probably inclinometer problem.

### Telemetry

MTM1M3\_logevent\_inclinometerWarning | sensorReportsIllegalFunction

### Setting override

InclinometerSettings | FaultOnSensorReportsIllegalFunction

<a name="0x00030008"></a>
## InclinometerUnknownProblem - 0x00030008 (196616)

Modbus exception code is not 1 or 2 (see
https://simplymodbus.ca/learn-exceptions.html for list of Exception Codes).

Very unlikely to occur. If it occurs, it is probably inclinometer problem.

### Telemetry

MTM1M3\_logevent\_inclinometerWarning | unknownProblem

MTM1M3\_inclinometerData | inclinometerAngle

### Setting override

InclinometerSettings | FaultOnUnknownProblem
