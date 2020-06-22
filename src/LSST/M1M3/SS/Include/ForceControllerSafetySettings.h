#ifndef FORCECONTROLLERSAFETYSETTINGS_H_
#define FORCECONTROLLERSAFETYSETTINGS_H_

struct ForceControllerSafetySettings {
    bool FaultOnSafetyLimit;
    bool FaultOnXMomentLimit;
    bool FaultOnYMomentLimit;
    bool FaultOnZMomentLimit;
    bool FaultOnNearNeighborCheck;
    bool FaultOnMagnitudeLimit;
    bool FaultOnFarNeighborCheck;
    bool FaultOnElevationForceClipping;
    bool FaultOnAzimuthForceClipping;
    bool FaultOnThermalForceClipping;
    bool FaultOnBalanceForceClipping;
    bool FaultOnAccelerationForceClipping;
    bool FaultOnActiveOpticNetForceCheck;
    bool FaultOnActiveOpticForceClipping;
    bool FaultOnStaticForceClipping;
    bool FaultOnAberrationNetForceCheck;
    bool FaultOnAberrationForceClipping;
    bool FaultOnOffsetForceClipping;
    bool FaultOnVelocityForceClipping;
    bool FaultOnForceClipping;
};

#endif /* FORCECONTROLLERSAFETYSETTINGS_H_ */
