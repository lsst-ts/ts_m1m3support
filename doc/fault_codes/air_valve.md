# Air Controller - Air Valve (0x00010000 mask)

Those errors occurs due to mismatch between commanded and desired master air
valve state. There is only one, electronically controlled air valve. 

## Severity 

Most likely air valve will remain closed or cannot be closed. Mirror shall not
be operated with this failure.

M1M3 support system cannot operate without air - so it cannot operate when the
valve is closed. Keeping the valve open can be dangerous, as this is the last
line of defence against jammed force actuator valve. Even as the whole scenario
is highly unlikely, we warn against using overrides to let the system operate
with the following errors.

## Rectification

Check master air valve signals and wiring. Check air valve - does it really opens and
closes as commanded? You can use pressure inside the system (measured on
hardpoints) to confirm valve true state.

<a name="0x00010001"></a>
## AirControllerCommandOutputMismatch - 0x00010001 (65537)

Occurs when commanded value to open the master air valve doesn't match sensed
command state. Most likely indicates either problem in the valve or wiring.
Overriding the problem shall be futile - the mirror cannot be operated without
air.

### Telemetry

MTM1M3\_logevent\_airSupplyStatus | airCommandedOn, airCommandOutputOn

MTM1M3\_hardpointMonitorData | breakawayPressure

MTAirCompressor\_analogData | linePressure

### Setting override

AirControllerSettings | FaultOnCommandOutputMismatch

<a name="0x00010002"></a>
## AirControllerCommandSensorMismatch - 0x00010002 (65538)

Occurs when the master air valve sensor value doesn't match expected
(commanded) air valve state. Air valve needs about 30 seconds to change state
(open or close). There is a sensor attached to the valve, which is logical 1
when air valve is closed.

### Telemetry

MTM1M3\_logevent\_airSupplyStatus | airCommandedOn, airValveOpened, airValveClosed

MTM1M3\_hardpointMonitorData | breakawayPressure

MTAirCompressor\_analogData | linePressure

### Setting override

AirControllerSettings | FaultOnCommandSensorMismatch
