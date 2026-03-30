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

## Configuration

The force applied during push and pulls can be configured under
ForceActuatorSettings|BumpTest in the configuration file. Default value is 222
N for push and -222 N for pull force. Also warning and error levels for RMS
values can be adjusted in the configuration file. See the following snippet for
details. Values from the file are referenced in 'text', such as 'PushForce'.

```
ForceActuatorSettings:
  # Force Actuator Bump Tests values.
  BumpTest:
    # Force offset for push (positive) bump test. Must be in (0, 222] range.
    PushForce: 210
    # Force offset for pull (negative) bump test. Must be in [-222, 0) range.
    PullForce: -210
    # Tolerances for FA being tested.
    TestedTolerances:
      # Warning tolerance for FAs that are bump tested.
      Warning: 2.5
      # Error tolerance for FAs that are bump tested.
      Error: 5
    # Tolerances for FA not tested. Used for discovering deviations from 0 N
    # force the units shall measure, when the FA is not being tested.
    NonTestedTolerances:
      # Warning tolerance for FAs that are not bump tested.
      Warning: 5.2
      # Error tolerance for FAs that are not bump tested.
      Error: 6
    # Time (in seconds) for the bump test forces to settle down. If the FA RMS
    # aren't settled before timeout expires, a bump test error is signaled.
    SettleTime: 3.5
    # Number of measurements the RMS is calculated. The systems runs in 50 Hz
    # loop, so there are 50 measurements per second.
    Measurements: 100
    # Minimal distance for FA being parallel bump tests (tested at the same
    # time). In meters, minimal allowed is 2 meters.
    MinimalDistance: 4
```

## Execution

The test is initiated via the `forceActuatorBumpTest` SAL command. It can be run on
a single actuator, a specific cylinder, or cycled through the entire array.

1. **Phase 1 (Push):** Apply 'PushForce' force.

2. **Phase 2 (Pull):** Apply 'PullForce' force.

3. **Monitoring:** The CSC monitors the load cell feedback against the
   commanded value.

As tests can be run in parallel, multiple FAs can be tested at the same time.
The algorithm threads differently FAs that are being tested and non-tested FAs.

FAs being tested must produce desired force output, within margin
'TestedTolerances' margins.

Non-tested FAs are required to stay at 0 N measured force during test
execution. When a FA ends test phase, non-tested FAs statistics RMS are checked.

Usually up to 4 FAs tests are running in parallel. The number depends on
'MinimalDistance' and the algorithm selecting FAs to tests. A trivial greedy
algorithm is implemented in *ts_m1m3_utils* *BumpTestRunner* class.

## Acceptance Criteria

A test is marked as **PASSED** (State 6) if all the following conditions are
met:

* The Force Actuator being tested following error RMS stays within the
  'TestedTolerances/Error' for 'Measurements' seconds within 'SettleTime'
  seconds window from the time actuator forces were changed.

* Non-tested force actuators following error RMS is outside of
  'NonTestedTolerances/Error' in the last 'Measurements' period.

Thus, a test is marked as **FAILED** (State 7 and above) if any of the following
conditions is met:

* The tested force actuator following error RMS does not drop below
  'TestedTolerances/Error' and stays in limits for 'Measurements' measurements
  within the 'SettleTime' period.

* Non-tested force actuators following error RMS is outside of
  'NonTestedTolerances/Error' in the last 'Measurements' period, counted from
  the time the FA passed the test.

## Handling Failures

* **Other FA Failure:** Best detected in EUI (M1M3GUI), or
  forceActuatorBumpTestStatistics events. EUI shows under statistics tab force
  actuators which caused test to fail.

* **Single Failure:** Often caused by transient pneumatic lag or stiction.
  Clear the fault and re-run the test for that specific actuator.

* **Double Failure:** If the same actuator fails twice, it is flagged for
  maintenance. This typically indicates a faulty pressure servo valve.

* **Telemetry Check:** Review the EFD topic
  `lsst.sal.MTM1M3.logevent_logMessage` for strings containing "Failed FA" to
see the exact measured deviation (e.g., "measured force plus (215.3) is too far
from 222±5").

## Data Access

* **Status:** `lsst.sal.MTM1M3.logevent_forceActuatorBumpTestStatus`

Querying this for FA bump test state TESTINGPOSITIVE state is the easiest
way to find out when the bump tests started. Within 60 seconds of this tests
shall come a message with PASSED status, or some of the FAILED status.

* **Statistics:** `lsst.sal.MTM1M3.logevent_forceActuatorBumpTestStatistics`

Contains tests statistics. That shall be queried for minimal, maximal, average
and RMS of the forces during the tests.

* **Force Data:** `lsst.sal.MTM1M3.forceActuatorData`

Contains raw data. Can be looked for detailed informations about the force
measured by the FAs during the tests.

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

## MinimalDistance

Minimal distance for FA being parallel bump tests (tested at the same time). In
meters, minimal allowed is 2 meters.
