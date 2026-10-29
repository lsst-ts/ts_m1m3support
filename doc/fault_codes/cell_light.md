# Cell Light (mask 0x00060000)

Lights inside the mirror cell can be remotely controlled by software. The
control was modified by addition of a manual switch, which is used by
summit technicians when they are working inside the mirror cell.

Mirror cell lights shall be powered off during observation, as this is an obvious
stray light source, contaminating the acquired night data.

## Severity

Can be ignored if not reparable. Usually caused by wrongly set switch on the
cell doors - flipping it shall solve the problem. Always try to make sure
lights are off before opening the dome doors, regardless of what software
tells.

## CellLightOutputMismatch - 0x00060001 (393217)

The control software controls light power, and also sense back if the power is
on or off. Any mismatch between what is commanded and what is read back will
cause this error.

### Rectification

Most likely hardware problem, either with lights commanding switch, or feedback
sensor. Requires electrician to investigate and fix the problem.

### Telemetry

MTM1M3\_logevent\_cellLightState | cellLightsCommandedOn, cellLightsOutputOn, cellLightsOn

MTM1M3\_logevent\_cellLightWarning | outputMismatch

### Setting override

CellLightSettings | FaultOnOutputMismatch

## CellLightSensorMismatch - 0x00060002 (393218)

## Rectification

Check if the lights are really on inside the cell. Flip white light switch
located left from the entrance door. Sensor issue is extremely unlikely - but
feel free to override fault in config file if flipping the switch doesn't help.

### Telemetry

MTM1M3\_logevent\_cellLightState | cellLightsCommandedOn, cellLightsOutputOn, cellLightsOn

MTM1M3\_logevent\_cellLightWarning | sensorMismatch

### Setting override

CellLightSettings | FaultOnSensorMismatch
