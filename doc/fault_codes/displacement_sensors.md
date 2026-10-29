# Displacement Sensors - Independent Measurement System (IMS)

Those errors signal problem in IMS. IMS sensors are readout by a convertor -
DL-RS1A unit. Data are transmitted to control system via serial link from the
converter. Serial line is connected to cRIO serial module, and commanded by
FPGA code to readout data.

Please consult DL-RS1A manual, available on data sheet wiki page, for details.
You shall be able to hook your computer to serial line going to the DL-RS1A and
query it directly to find out details of the failure.

## Severity

As the IMS is optional, and isn't used for mirror control, suggested overrides
can be used to keep the mirror operational.

## Rectification

Check the DL-RS1A communication unit, its power and wiring (bothw wiring to
cRIO and to the displacement sensors). Audit changes to CSC and FPGA bitfile.
Replace DL-RS1A unit and/or a displacement sensor.

The unit is connected to NI-9870 module - slot 8 of the cRIO, port 2. The NI
module needs to be independently powered. The power to the module shall be
checked, and the module replaced. 

Inclinometer shares the same NI module to communicate with the hardware. If
inclinometer reports errors, chances are the NI module is the culprit.

<a name="0x00020001"></a>
## DisplacementSensorReportsInvalidCommand - 0x00020001 (131073)

Internal to displacement sensor logic (DL-RS1A controller). Per DL-RS1A, "Make
sure that the external device has sent a command listed in "Communication
commands" (page 11). As the command is always to read all data (FPGA doesn't
know any other command), occurrence of this error would either point to some
strange issue in the cRIO, FPGA software (bitfile) of DL-RS1A controller.

### Telemetry

MTM1M3\_logevent\_displacementSensorWarning | sensorReportsInvalidCommand

MTM1M3\_imsData

### Setting override

DisplacementSettings | FaultOnSensorReportsInvalidCommand

<a name="0x00020002"></a>
## DisplacementSensorReportsCommunicationTimeoutError - 0x00020002 (131074)

Reported by the DL-R51A unit, suggest problem of the controller communicating
with the sensors. Communication could not be established between the DL and the
amplifier.

### Rectification

Per RL-RS1A manual, check if the GT2-100 is not in the initial reset process or
reflecting the valid ID setting.

### Telemetry

MTM1M3\_logevent\_displacementSensorWarning | sensorReportsCommunicationTimeoutError

MTM1M3\_imsData

### Setting override

DisplacementSettings | FaultOnSensorReportsCommunicationTimeoutError

<a name="0x00020003"></a>
## DisplacementSensorReportsDataLengthError  - 0x00020003 (131075)

Data with the correct length was not received. Not expected, as the CSC doesn't
send any data to IMS unit - it sends only requests to retrieve data.

### Telemetry

MTM1M3\_logevent\_displacementSensorWarning | sensorReportsDataLenghtError

MTM1M3\_imsData

### Setting override

DisplacementSettings | FaultOnSensorReportsDataLengthError

<a name="0x00020004"></a>
## DisplacementSensorReportsNumberOfParametersError - 0x00020004 (131076)

The correct number of parameters for the command was not received. Not expected, as the CSC doesn't
send any data to IMS unit - it sends only requests to retrieve data.

### Telemetry

MTM1M3\_logevent\_displacementSensorWarning | sensorReportsNumberOfParametersError

MTM1M3\_imsData

### Setting override

DisplacementSettings | FaultOnSensorReportsNumberOfParametersError

<a name="0x00020005"></a>
## DisplacementSensorReportsParameterError - 0x00020005 (131077)

* A parameter exceeds its range of value.
* The external device is trying to write a data type that cannot be written.
* The external device is trying to read a data type that cannot be read.
* The data format is incorrect

Not expected, as the CSC doesn't send any data to IMS unit - it sends only
requests to retrieve data.

### Telemetry

MTM1M3\_logevent\_displacementSensorWarning | sensorReportsParameterError

MTM1M3\_imsData

### Setting override

DisplacementSettings | FaultOnSensorReportsParameterError

<a name="0x00020006"></a>
## DisplacementSensorReportsCommunicationError - 0x00020006 (131078)

An error was detected with sensor RS-232C communication.

### Rectification

Make sure that DL-RS1A and the external device have the same communication
settings configured. For information on configuring DL-RS1A, refer to page 2 of
the DL-RS1A user manual (available on Wiki Datasheets page).

### Telemetry

MTM1M3\_logevent\_displacementSensorWarning | sensorReportsCommunicationError

MTM1M3\_imsData

### Setting override

DisplacementSettings | FaultOnSensorReportsCommunicationError

<a name="0x00020007"></a>
## DisplacementSensorReportsIDNumberError - 0x00020007 (131079)

The ID number specified with the command is incorrect. Not expected, as the CSC
doesn't send any data to IMS unit - it sends only requests to retrieve all data.

### Telemetry

MTM1M3\_logevent\_displacementSensorWarning | sensorReportsIDNumberError

MTM1M3\_imsData

### Setting override

DisplacementSettings | FaultOnSensorReportsIDNumberError

<a name="0x00020008"></a>
## DisplacementSensorReportsExpansionLineError - 0x00020008 (131080)

The communication could not be established due to a problem with an expansion line.

### Rectification

Check that each of the sensor amplifiers and DL-RS1A are securely and properly
connected by referring to "Connecting the Unit to Sensor Amplifiers" (page 3i
of DL-RS1A User manual - in Wiki Datasheets).

Make sure that sensor amplifiers that are supported by DL-RS1A are connected
(refer to page 4 of the DL-RS1A User manual). Check if the GT2-100 is not in the
initial reset process or reflecting the valid ID setting

### Telemetry

MTM1M3\_logevent\_displacementSensorWarning | sensorReportsExpansionLineError

MTM1M3\_imsData

### Setting override

DisplacementSettings | FaultOnSensorReportsExpansionLineError

<a name="0x00020009"></a>
## DisplacementSensorReportsWriteControlError - 0x00020009 (131081)

DL-RS1A is not writable. Not expected, as the CSC doesn't send any data to IMS
unit - it sends only requests to retrieve data.

### Telemetry

MTM1M3\_logevent\_displacementSensorWarning | sensorReportsWriteControlError

MTM1M3\_imsData

### Setting override

DisplacementSettings | FaultOnSensorReportsWriteControlError

<a name="0x0002000A"></a>
## DisplacementResponseTimeoutError - 0x0002000A (131082)

Indicates timeout in communication between the cRIO NI-9870 serial module and
DL-RS1A IMS unit.

### Telemetry

MTM1M3\_logevent\_displacementSensorWarning | responseTimeoutError

MTM1M3\_imsData

## Setting override

DisplacementSettings | FaultOnResponseTimeoutError

<a name="0x0002000B"></a>
## DisplacementInvalidLength - 0x0002000B (131083)

Triggered on invalid length of reply from the DL-RS1A. Most likely cause would
be some DL-RS1A problems. This shall be similar to timeout problem (error
0x2000A).

### Telemetry

MTM1M3\_logevent\__displacementSensorWarning | invalidLength

MTM1M3\_imsData

### Setting override

DisplacementSettings | FaultOnInvalidLength

<a name="0x0002000C"></a>
## DisplacementUnknownCommand - 0x0002000C (131084)

Wrong reply from the DL-RS1A unit.

### Telemetry

MTM1M3\_logevent\_displacementSensorWarning | unknownCommand

MTM1M3\_imsData

### Setting override

DisplacementSettings | FaultOnUnknownCommand

<a name="0x0002000D"></a>
## DisplacementUnknownProblem - 0x0002000D (131085)

Most likely problem in DL-RS1A communication.

### Telemetry

MTM1M3\_logevent\_displacementSensorWarning | unknownProblem

MTM1M3\_imsData

### Setting override

DisplacementSettings | FaultOnUnknownProblem

<a name="0x0002000E"></a>
## DisplacementInvalidResponse - 0x0002000E (131086)

Wrong reply from the DL-RS1A unit.

### Telemetry

MTM1M3\_logevent\_displacementSensorWarning | invalidResponse

MTM1M3\_imsData

### Setting override

DisplacementSettings | FaultOnInvalidResponse
