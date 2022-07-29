# M1M3 Support Software

Contains soft real-time software which runs on NI cRIO for M1M3 support.
[FPGA](https://github.com/lsst-ts/ts_m1m3supportFPGA) handling low-level
communication, and 
[LabView EUI (GUI panel)](https://github.com/lsst-ts/ts_m1m3supportEUI) running
on host PC are separate projects.

## Theory of operation

M1M3 Support System (M1M3SS) operates Dual Axis Actuators (DAAs) and Single
Axis Actuators (SAAs) to keep mirror in neutral buoyancy state, counteracting
primary gravity force on the mirror. Hardpoints forming hexapod are used to
position the mirror. The control algorithm offloads force from top of the
hardpoints into force actuators, so force reported by load cells placed on top
of the hardpoints shall be close to 0 during normal mirror operations.

Force actuator target force is calculated as sum of the following components:

### Acceleration

Forces calculated from X, Y and Z acceleration measured by 8 one axis
accelerometers placed inside mirror cell (hence using the same reference system
as the mirror cell). The force offsets are calculated using LUT, which assumes
linear dependence of acceleration and resulting force. Input acceleration is in
m^2/s.

### Active Optic

Calculated and provided by AOS. Set with ApplyActiveOpticsForces, reset with
ClearActiveOpticsForces. AOS process wave front sensing data, fits coefficient
of the Zernike polynomials to produce Zernikes, and those are transformed into
mirror deformation. The deformation is corrected by applying force offsets in
Z direction. The force offsets (156 vector for 156 force actuators) are passed
(in N) as parameter of the command. Forces resulting from transformation using
the table are assumed to be in mN, so need to be divided by 1000.0 to get N
forces.

### Azimuth

Calculated from look up tables from telescope azimuth. Six values are stored into
look-up table for X,Y and Z direction forces, representing five Five degree
polynomial coefficients (thus enabling close approximation to goniometrical
functions). The coefficient should be empirically fitted. Resulting forces are
in N.

### Balance

Balances hardpoint force excess. Takes in hexapod forces and moments calculated
from excess forces, and produces force to be applied on force actuators.
Resulting forces are in N.

### Elevation

Calculates gravity vector (elevation) compensation. As gravity is the major
force acting on the mirror, this is the biggest force contributor. Similar to
azimuth, the resulting force offsets are calculated using fifth degree
polynomial. Fifth degree polynomial is enough to estimate sin and cos to more
then 2 decimal points - and force distribution naturally resembles gravity
vector projection on X and Y axis. Elevation is provided in degrees (0 on
horizon, 90 in zenith). Resulting forces are in N.

### Offset

Operator supplied offset forces. Operator specify offsets in X, Y and Z, and
corresponding offsets moments. Those are distributed using the same
transformation used for Balance forces. Resulting forces are in N.

### Static

Static forces to bend mirror close to optimal optic shape. Those are measured
in a lab and stored in configuration tables. Resulting forces are in N.

### Thermal

Calculate correction forces for temperature. Fifth degree polynomial fit is
assumed. Input is temperature in degrees centigrade, output is force in N.

### Velocity

Similar to acceleration forces. Cell velocity is provided by accelerometers
placed inside mirror cell. Linear dependence between force and cell velocity is
assumed.

DAAs and SAAs are also equipped with booster valves. Those are activated to
make the actuator stiffer during mount accelerations.

## Safety

Mirror is precise. Excessive force loads can break it. Control software must
protect mirror from such forces.

The safest configuration for the mirror is when it's resting on supporting
coils in passive state. Any failure or detected problem (seismic event,..)
shall put mirror quickly to lowered position. Of course the controller and cell
hardware shall not drop the mirror into a free fall. Emergency lowering of the
mirror is referred as cell panic in the following text.

DAA and SAA are equipped with force cells. Difference between target and
measured force is measured and excessive values leads to cell panic.

Hardpoints are equipped with pneumatic driven break-away mechanism. Any
excessive force will make hardpoint break away, and once detected with control
software, mirror goes into panic. Mirror is still safe with any number of loose
hardpoints, resting on SAA and DAA.

Any miss-communication, unexpected behavior of either hardpoint, SAA or DAA
shall bring mirror into panic.

## NI-PC interaction

NI cRIO is running FPGA code programmed in LabVIEW and C++ binary code. FPGA
code handles details of low-level modbus communication. C++ code handles
communication over SAL with TCS, and communicates with FPGA with a command FIFO
- see writeCommandFIFO and readCommandFIFO calls:

```cpp
bool FirmwareUpdate::ClearFaults(int subnet, int address) {
        this->desiredState = 3;
        ModbusBuffer buffer;
        this->SetupBuffer(&buffer, subnet);
        buffer.writeU8((uint8_t)address);
        buffer.writeU8(65);
        buffer.writeU8(0);
        buffer.writeU8(5);
        buffer.writeCRC(4);
        buffer.writeEndOfFrame();
        buffer.writeWaitForRx(10000);
        this->EndBuffer(&buffer);
        this->fpga->writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
        return this->ProcessResponse(&buffer, subnet);
}
```

The command is processed inside FPGA, put into command queue for serial/modbus
port, send over the serial line to Inner Loop Controllers (ILC), response is
read, added to response queue, added to command response queue and finally
readout.

## Directories

* *Bitfiles* - Contains the bit files that are loaded onto the FPGA's
* *SettingFiles* - Default configuration files
* *src* - Contains the C++ code for the ts_M1M3Support application
* *tests* - Contains python tests for the ts_M1M3Support application
* *Utilities* - Contains the code for the utility applications

## Included external libraries

* **src/spdlog** [SpeedLog](https://github.com/gabime/spdlog) library (see
  [version.h](src/spdlog/version.h) for version)

This software depends on the fmt lib (MIT License),
and users must comply to its license: https://github.com/fmtlib/fmt/blob/main/LICENSE.rst

# Building

Please use lsstts/mtm1m3\_sim Docker container to run simulator.

## Setting up the build environment

```bash
BASE=$(pwd)/M1M3SS
mkdir -p $BASE
git clone git@github.com:lsst-ts/ts_m1m3support
git clone git@github.com:lsst-ts/ts_sal
git clone git@github.com:lsst-ts/ts_opensplice
git clone git@github.com:lsst-ts/ts_xml
source ts_sal/setup.env
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${BASE}/ts_sal/test/lib
cd ts_sal/test
cp -v ../../ts_xml/sal_interfaces/*.xml .
cp -v ../../ts_xml/sal_interfaces/MTM1M3/*.xml .
cp -v ../../ts_xml/sal_interfaces/MTMount/*.xml .
salgenerator MTM1M3 generate cpp
salgenerator MTM1M3 generate python
salgenerator MTMount generate cpp
salgenerator MTMount generate python
```

# Configuring NI cRIOs for deployment

The base cRIO must be configured and up and running. OpenSpliceDDS shared
libraries must reside in path searched for dynamic libraries. SAL libraries are
statically linked into CSC binary, so those aren't needed on cRIO.

ExpansionFPGA resource name (set in NI MAX, under resource name) must match the
name specified in ExpansionFPGApplicationSettings under resource. This URI also
contains expansion cRIO IP address:

rio://139.229.178.185/M1M3-SupportExpansion-RIO

## Compiling for debugging

Please supply "DEBUG=1" to make call to compile with debug data (-g flag).

**Please be aware that cRIO memory usually isn't sufficient to compile code
with debug data. Compiling code without debug data, e.g. no DEBUG=1 option,
works on cRIO.**

## Compiling for deployment

Ni RT stack isn't needed.

```bash
cd $BASE
source ts_sal/setup.env
cd ts_m1m3support
make
```

## Deploying the code

```bash
cd $BASE
source ts_sal/setup.env
cd ts_m1m3support
make ipk
```

Copy and install ipk on cRIO with opkg install command _(if needed, use
--force-reinstall to overwrite previous package version)_.

## Compiling for simulation

If you run make before (for deployment), you need to run make clean before
compiling for simulator.

```bash
cd $BASE
source ts_sal/setup.env
cd ts_m1m3support
make SIMULATOR=1
```

## Simulator limitations

Simulator does reasonable job computing various feedback signals. Those signals
(values) and commended values are know to not be computed correctly:

* **HP measured forces** those are hard to calculate. Direct consequences is
that HP chasing doesn't work.

* * **HP chasing steps** and **mirror position** as force computation doesn't
work, HP and mirror position is not to be trusted in simulator as the mirror is
raised or lowered. HP are send to target position after raise is completed
(mirror weight is 100% completed), so mirror position is reported correctly
when mirror is in active state.

## Running in deployment

With the exception of OpenSpliceDDS libraries, which needs to be installed from
third party provided repository, opkg package contains everything needed to run
the CSC on cRIO. Standard startup script is provided, so:

```bash
/etc/init.d/ts-M1M3supportd restart
```

shall (re)start the CSC. In case you prefere to run (for debugging,..) CSC from
the terminal, setup the environment by:

```bash
set +a
source /etc/default/ts-M1M3support
set -a
```

then you can start CSC with:

```bash
ts-M1M3supportd -c /var/lib/ts-M1M3support
```

## Running m1m3cli

M1M3 command line client can be run (after you stop CSC)  as:

```bash
/etc/init.d/ts-M1M3supportd stop # not needed if CSC isn't running
m1m3sscli
Please type help for more help.
M1M3 > help
```

SAL isn't needed to run the command line tool.

## Running in simulation

After make SIMULATOR=1, you can run the code as simulator. This doesn't need
any hardware attached.

```bash
source ./ts_sal/setup.env && ./ts_m1m3support/ts_M1M3Support -c SettingFiles
```

The simulator does a pretty good job giving realistic data, you will notice
some data isn't populated, it may be added at a later date, it may not. When
issuing state change commands make sure to specify "value" as otherwise the
command will be rejected.
