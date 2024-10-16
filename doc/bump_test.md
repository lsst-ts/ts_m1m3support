# Bump test algorithm

The basic scope of the algorithm is impemented and documented in the
[BumpTestController](https://ts-m1m3support.lsst.io/classLSST_1_1M1M3_1_1SS_1_1BumpTestController.html)
class.

## Stages

The following stages are 


The current code record and evaluates configured number of measurement. Average
is used as a metric comparing measured values with tolerances.


# Settings

Settings involved are in the configuration file, under ForceActuatorSettings /
BumpTest. Following is a sample with default values:

```
  BumpTest:
    TestedTolerances:
      Warning: 2.5
      Error: 5
    NonTestedTolerances:
      Warning: 20
      Error: 100
    SettleTime: 3.0
    Measurements: 10
```

The settings are described in the next paragraph:

## TestedTolerances

Sets tolerance for the force actuator being tested. This shall be more strict
than the following error tolerances for the force actuator not tested.

### Warning

If the force actuator following error is above the warning limit, a warning is
printed to the system log.

### Error

If any absolute value of the following error (deviation) of tested force
actuator exceededs the Error value, an error is triggered and the bump test
fails. Message produced and data recorded shows which force actuator caused the
test failure.

## NonTestedTolerances

Sets allowable deviation (maximal absolute value) for non-tested force
actuators. Those tests are performed to make sure the other, not moved force
actuators, are capable to hold steady when surrounding force actuator is being
moved.

### Warning

If the force actuator following error is above the warning limit, a warning is
printed to the system log and is flagged in map of the bump tests results.

### Error

If any absolute value of the following error (deviation) of not tested force
actuators is exceeded, an error is triggered and the bump test fails. Message
produced and data recorded shows which force actuator caused the test failure.

## SettleTime

Time in seconds for measurement to settle. If that time expires from the last
change of the force actuator forces, and at least **Measurements** values were
collected, the testing period can begin.

## Measurements

Number of measurements to test.
