# Interlock (0x00040000 mask)

The following faults are triggered by interlocks. Those are the most common
problems - as the M1M3 operation is blocked by something that says the mirror
cannot be safely operated.

## Severity

Mirror support system shall not be operated with any active interlock. Priority
shall be given to fix the problem then overriding the interlock condition.

## Rectification

Fix the interlock cause and reset the interlock.

<a name="0x00040001"></a>
## InterlockHeartbeatStateOutputMismatch - 0x00040001 (262145)

Occurs when sensed interlock heartbeat signal output is different to commanded heartbeat signal.

### Rectification

This is most likely software issue. If it ever occurs, it means the code
sending signal out wasn't executed, but the code switching commanded heartbeat
boolean variable was executed.

If it ever occurs, check if an updated CSC version was installed. Try to run
some old CSC to see if the problem reappears. Code to switch states can be
tracked in CSC C++ source code, searching for provided telemetry items -
heartbeatStateOutputMismatch, heartbeatCommandedState and heartbeatOutputState.

### Telemetry

MTM1M3\_logevent\_interlockWarning | heartbeatStateOutputMismatch

MTM1M3\_logevent\_interlockStatus | heartbeatCommandedState, heartbeatOutputState

### Setting override

InterlockSettings | FaultOnHeartbeatStateOutputMismatch

<a name="0x00040002"></a>
## InterlockCriticalFaultStateOutputMismatch - 0x00040002 (262146)

Not used.

<a name="0x00040003"></a>
## InterlockMirrorLoweringRaisingStateOutputMismatch - 0x00040003 (262147)

Not used.

<a name="0x00040004"></a>
## InterlockMirrorParkedStateOutputMismatch - 0x00040004 (262148)

Not used.

<a name="0x00040005"></a>
## InterlockAuxPowerNetworksOff - 0x00040005 (262149)

Activated when power is not supplied to the force actuators valves. This opens
3-way solenoid valves, so the pressurized air inside cylinders bleeds out
through flow restrictor orifice.

### Telemetry

MTM1M3\_logevent\_interlockSystemFault | powerNetworksOff

### Setting override

InterlockSettings | FaultOnAuxPowerNetworksOff

<a name="0x00040006"></a>
## InterlockThermalEquipmentOff - 0x00040006 (262150)

This is in fact mirror doors interlock, signaling the mirror cell doors are
open. For reasons we were unable to identify, GIS sometimes briefly triggers
this interlock during observation. As this is a human-safety interlock,
designed to prevent mirror operations when somebody is inside the mirror cell,
it is usually ignored in GIS.

Extra caution needs to be carried when mirror cell doors are opened to allow
access and work inside the cell. Afternoon walk around shall verify mirror cell
is clear of any personal and equipment.

### Rectification

Usually resetting CSC would be enough to fix the problem - the interlock
triggers only briefly.

### Telemetry

MTM1M3\_logevent\_interlockSystemFault | thermalEquipmentOff

### Setting override

InterlockSettings | FaultOnThermalEquipmentOff

<a name="0x00040007"></a>
## InterlockLaserTrackerOff - 0x00040007 (262151)

Not used.

<a name="0x00040008"></a>
## InterlockAirSupplyOff - 0x00040008 (262152)

GIS is overriding M1M3 CSC control, closes air valve and opens bleed valve.

### Telemetry

MTM1M3\_logevent\_interlockSystemFault | airSupplyOff

### Setting override

SafetySettings | FaultOnAirSupplyOff

<a name="0x00040009"></a>
## InterlockGISEarthquake - 0x00040009 (262153)

Not used.

<a name="0x0004000A"></a>
## InterlockGISEStop - 0x0004000A (262154)

Not used.

<a name="0x0004000B"></a>
## InterlockTMAMotionStop - 0x0004000B (262155)

TMA movement was stopped. This usually happens when TMA emergency breaks are
activated.

### Rectification

Clear all TMA interlocks.

### Telemetry

MTM1M3\_logevent\_interlockSystemFault | tmaMotionStop

### Setting override

SafetySettings | FaultOnTMAMotionStop

<a name="0x0004000C"></a>
## InterlockGISHeartbeatLost - 0x0004000C (262156)

GIS heartbeat was lost. Either GIS issue, or a wiring issue.

### Telemetry

MTM1M3\_logevent\_interlockSystemFault | gisHeartbeatLost

### Setting override

SafetySettings | FaultOnGISHeartbeatLost
