# Force Controller (mask 0x00050000)

Force controller functions are to calculate, apply and check measured forces and
moments acting on the glass mirror.

Calculated forces are check for know safety problems (details are provided in
the UofA memo 95-II). If all checks are passed, forces are applied to the glass
mirror. Measured mirror forces are converted to total mirror forces and
moments, and those are checked against specified limits.

## ForceControllerSafetyLimit - 0x00050001 (327681)

Occurs when any calculated (to be applied) cylinder force is outside allowed
safety limit. Violation of the individual force actuators limits is published
in MTM1M3\_logevent\_forceSetpointWarning safetyLimitWarning array (156
booleans, index is the force actuator index).

Limits are specified in CylinderLimitPrimaryTable and CylinderLimitSecondaryTable.

### Severity

This is critical fault. Its occurrence signals serious problems in how the
cylinder forces are calculated.

The current set limits are multiply of the maximum force the force actuator can
produce. If this error occurs, it signals really serious problems in the CSC
calculations.

### Rectification

Find out which force actuator(s) caused the error to occur - safetyLimitWarning
set to true shall indicate failed force actuator. Look on its
preclippedCylinderForces primaryCylinderForces and secondaryCylinderForces
values and compare those with values in CylinderLimit tables.

If possible, leave analysis to someone familiar with the code. This error is
unexpected and its occurence shall be regarded as highly suspicious.

### Telemetry

MTM1M3\_logevent\_preclippedCylinderForces | primaryCylinderForces, secondaryCylinderForces

MTM1M3\_logevent\_forceSetpointWarning | safetyLimitWarning

MTM1M3\_logevent\_forceControllerFault | safetyLimit

### Setting override

ForceControllerSettings | FaultOnSafetyLimit

## ForceControllerXMomentLimit - 0x00050002 (327682)

Calculated X moment of the force actuator forces crossed pre-set limit. The
limit moment is calculated from configuration values specified in
ForceActuatorSettings, as MirrorXMoment * SetpointXMomentLowLimitFactor and
MirrorXMoment * SetpointXMomentHighLimitFactor.

### Severity

This error usually occurs with one or more force actuators disabled. If it
occurs with all force actuators running, it might signal a serious issue with
mirror or TMA (if it shows during movement).

### Rectification

This error usually occurs with one or more force actuators disabled. Reducing
maximal telescope speed shall fix the problem.

If the error shows with all force actuator disabled, it is either caused by
irregular TMA movement, or serious problems with the mirror. Check for
irregularities in the TMA movement. Unless you don't see any irregular movement
when X moment went outside of the limits, stop observations and wait for
analysis from M1M3 team.

### Telemetry

MTM1M3\_forceActuatorData | mx

MTM1M3\_logevent\_forceControllerFault | xMomentLimit

### Setting override

ForceControllerSettings | FaultOnXMomentLimit

### Configuration

ForceActuatorSettings | MirrorXMoment, SetpointXMomentLowLimitFactor, SetpointXMomentHighLimitFactor

## ForceControllerYMomentLimit - 0x00050003 (327683)

Calculated Y moment of the force actuator forces crossed pre-set limit. The
limit moment is calculated from configuration values specified in
ForceActuatorSettings, as MirrorYMoment * SetpointYMomentLowLimitFactor and
MirrorYMoment * SetpointYMomentHighLimitFactor.

### Severity

This error usually occurs with one or more force actuators disabled. If it
occurs with all force actuators running, it might signal a serious issue with
mirror or TMA (if it shows during movement).

### Rectification

This error usually occurs with one or more force actuators disabled. Reducing
maximal telescope speed shall fix the problem.

If the error shows with all force actuator disabled, it is either caused by
irregular TMA movement, or serious problems with the mirror. Check for
irregularities in the TMA movement. Unless you don't see any irregular movement
when X moment went outside of the limits, stop observations and wait for
analysis from M1M3 team.

### Telemetry

MTM1M3\_forceActuatorData | my

MTM1M3\_logevent\_forceControllerFault | yMomentLimit

### Setting override

ForceControllerSettings | FaultOnYMomentLimit

### Configuration

ForceControllerSettings | MirrorYMoment, SetpointYMomentLowLimitFactor, SetpointYMomentHighLimitFactor

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
