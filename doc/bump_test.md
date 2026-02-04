# M1M3 Force Actuator Bump Test

## Overview

The Bump Test is a quasi-static diagnostic used to verify the health of the
M1M3 pneumatic force actuators. It ensures each actuator can accurately apply
and measure forces before the mirror is raised.

## Operational Prerequisites

* **System State:** The MTM1M3 CSC must be in the `ParkedEngineering` state.

* **Mirror Position:** The mirror must be fully lowered onto its static
  supports.

* **Safety:** Ensure no personnel are working inside the mirror cell during the
  test. Qualified engineers with appropriate training can be in the cell,
checking for possible actuators leaks.

## Execution

The test is initiated via the `forceActuatorBumpTest` SAL command. It can be run on
a single actuator, a specific cylinder, or cycled through the entire array.

1. **Phase 1 (Push):** Apply +222 N force.

2. **Phase 2 (Pull):** Apply -222 N force.

3. **Monitoring:** The CSC monitors the load cell feedback against the
   commanded value.

## Acceptance Criteria

A test is marked as **PASSED** (State 6) if:

* The following error RMS of the measured force is within **±5 N** of the
  target.

* The error stays within the tolerance for the duration of the measurement
  window in the `SettleTime` window.

A test is marked as **FAILED** (State 7 and above) if:

* The deviation exceeds 5 N.

* The actuator fails to reach the target force within the `SettleTime` period.

* Non-tested force actuators following error RMS is outside of **±6 N** in the
  last `Measurements` period.

## Handling Failures

* **Single Failure:** Often caused by transient pneumatic lag or stiction.
  Clear the fault and re-run the test for that specific actuator.

* **Double Failure:** If the same actuator fails twice, it is flagged for
  maintenance. This typically indicates a faulty pressure servo valve.

* **Telemetry Check:** Review the EFD topic
  `lsst.sal.MTM1M3.logevent_logMessage` for strings containing "Failed FA" to
see the exact measured deviation (e.g., "measured force plus (215.3) is too far
222±5").

## Data Access

* **Status:** `lsst.sal.MTM1M3.logevent_forceActuatorBumpTestStatus`

* **Statistics:** `lsst.sal.MTM1M3.logevent_forceActuatorBumpTestStatistics`

* **Force Data:** `lsst.sal.MTM1M3.forceActuatorData`

# M1M3 Force Actuator Bump Test Algorithm

The algorithm is implemented in the
[BumpTestController](https://ts-m1m3support.lsst.io/classLSST_1_1M1M3_1_1SS_1_1BumpTestController.html)
class, using functions provided by the FABumpTest class.

Tests can run in parallel. The FAs separated by a certain distance can be
tested simultaneously. The current implementation leaves scheduling on an
external application, commanding the tests. The code only check if FA is
significantly away from the other currently tested FAs. If that's the case, the
test is executed and command returns with success. If the requested FA is too
close to a FA currently tested, command returns with failure.

Settings stored under ForceActuatorSettings/BumpTest specify timeouts,
allowable ranges and number of measurements performed.

Bump test is performed in the following stages (per actuator axis - that's
done twice for DAA, and the tests don't need to run in succession:

1. check that RMS of the following error (difference between commanded and
measured force) in the last `Measurements` measured forces roughly equal to
0 (within `TestedTolerances/Error` for the FA being tested, and within
`NonTestedTolerances/Error` for FAs not tested) - the force test. If that's
not the case, continue collecting and processing measured forces for
`SettleTime` seconds. If even at the end of the interval the force test is
unsuccessful, record failure and continue with the following step.

2. apply a small positive force (222 N, `_testForce` member variable)

3. run the force test

4. null applied offset

5. run the force test

6. apply a small negative force (-222 N, - `_testForce` member variable)

7. run the force test

8. null applied offset

9. run the force test

If any of the steps fails, the failure is reported through
forceActuatorBumpTestStatistics and the test continue. All mirror safety
checks, except for the far and near neighbors checks, are still performed.
This ensure the mirror glass is never subject to excessive forces - even if
a misbehaving FA is connected and tested, immediate following error test
will fail the support system when excessive following error is measured.

`TestedTolerances/Warning` and `NonTestedTolerances/Warning` are used as
warning levels. If the error surpassed those, but doesn't trigger an error
(is not above error value), the problem is reported through
`forceActuatorBumpTestStatistics`.

## Execution Flow

The test logic for all actuators is handled in runLoop method. The loop
executes in the following steps:

1. Measured cylinder and axial forces are added to FABumpTestData cache by
   calling its `add_data` method.

2. Force statistics are computed for both cylinder (primary and secondary) and
   axial (X, Y and Z) forces.

3. Statistics for all FA is evaluated in `BumpTestController::_run_axis`
   method. If FA is being tested, FA statistics is compared with expected
values. If FA axis pass the test, the test ends, stage is set to the next test
step, and Test End Reporting is executed.

4. If test runs for too long, test failure is reported - see Test End
   Reporting.

5. If test stage ends, new forces are applied (as the `forceOffsets`) to the FA
   to continue with the next test stage.

## Parallel Bump Tests

Force actuators can be tested in parallel, if the distance from the another
bump-tested force actuator is greater than `MinimalDistance` (currently
**4m**).

Scheduling tests that all FAs are tested in the best time is a NP-complete
problem. One possible scheduling implementation is in `ts_m1m3_utils`
`BumpTestRunner` class.

## Test End Reporting

When test ends, the following is reported:

1. Always statistics of the tested FA axis - it's irrelevant, if the test is
   successful or failed. settleTime is set to NAN in case the test failed. This
is intended to simplify long-term monitoring, as those values are readably
available.

2. If secondary axis/force (primary/secondary, or Z/X or Y) failed, its
   statistic is reported. Stage field of the topic is increased by 50 for the
other axis/cylinder.

3. If any non-tested axis shows excessive values, those are also reported.  For
   this, the actuatorId and testType field are kept. Axis which showed excess
is recorded in stage field. See Test Statistics Stage Values for details.

The force actuator statistics is calculated from circular buffer. The buffer
length is specified in the :Measurements: configuration parameter.

## forceActuatorBumpTestStatistics event

The event fields are filled with the following values:

### actuatorId

Always the FA being tested - the FA for which the values belong.

### testType

One of the `BumpTestType` constant - Primary, Secondary, X, Y or Z. Describes
test of type being run.

### stage

The stage values encoded more than just stage to which the data belongs. The
value can as well signal problem with the other FA axis, or some other FA
showing excess during the testing period.

1. < 50 - BumpTest enumeration, describing state of the tested cylinder/axis.
   As RMS error can only be positive, negative overflows are recorded when the
error RMS condition is met and the average error value is negative.

2. 50 .. 100 - BumpTest enumeration for the other cylinder/axis. Signals
   problem on the other cylinder or axis.

3. 101 .. 999 - Problem is caused by some other FA primary cylinder, not tested
   (including parallel runs). This is the actuator ID of the off limits FA.

4. 1101 .. 1443 - Similar to above, for problem being the secondary cylinder.

5. 2101 .. 2443 - Similar to above, for problem being the X force.

6. 3101 .. 3443 - Similar to above, for problem being the Y force.

7. 4101 .. 4443 - Similar to above, for problem being the Z force.

### settleTime

Time lag of the FA. Time it takes for the FA to pass the test. In seconds. NAN
value indicates timeout - the FA specified either in actuatoroId or in stage
field failed the bump test.

### minimum

Minimum force recorded in tested interval.

### maximum

Maximum force recorded in tested interval.

### average

Calculated average force recorded during tested interval.

### errorRMS

Following error - difference between measured and commanded value - RMS (Root
Mean Square). Calculated as:

\f[ \sqrt{\sum_{n=1}^{measurements} fe^{2} } \f]

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
actuator exceeds the Error value, an error is triggered and the bump test
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

Number of measurements to test. Length of the circular buffer, holding last
n-values to compute test statistics.
