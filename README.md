# M1M3 Support Software

Contains realtime software which runs on NI cRIO for M1M3 support.
[FPGA](https://github.com/lsst-ts/ts_m1m3supportFPGA) handling low-level
communication, and 
[LabView EUI (GUI panel)](https://github.com/lsst-ts/ts_m1m3supportEUI) running
on host PC are separate projects.

## Theory of operation

M1M3 Support System (M1M3SS) operates Dual Axis Actuators (DAAs) and Single
Axis Actuators (SAAs) to keep mirror in neutral buoyancy state, counteracting
primary gravity force on the mirror. It uses hardpoints to steer the mirror,
primary when it's raised on controlled lowered. Hardpoints are also used to
make mirror stiff, removing wind induced oscillations.

There are secondary goals operating DAAs and SAAs:

* offload local hardpoints steering forces
* apply small forces to keep mirror in optically ideal shape

DAAs and SAAs are also equipped with booster valves. Those are activated to
counteract forces experienced during telescope slews.

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

NI cRIO is running FPGA code programmed in LabView and C++ binary code. FPGA
code handles details of low-level modbus communication. C++ code handles
communication over SLA with TCS, and communicates with FPGA with a command FIFO
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

The command is processed inside FPGA, put into command queue for modbus port,
send over modbus to Inner Loop Controllers (ILC), response is read, added to
response queue, added to command response queue and finally readout.

## Directories

* *Bitfiles* - Contains the bit files that are loaded onto the FPGA's
* *builds* - Contains utility applications for creating look up tables
* *SettingFiles* - Default configuration files
* *src* - Contains the C++ code for the ts_M1M3Support application
* *Tests* - Contains python tests for the ts_M1M3Support application
* *Utilities* - Contains the code for the utility applications

## Included external libraries

* **src/spdlog** [SpeedLog](https://github.com/gabime/spdlog) library (see
  [version.h](src/spdlog/version.h) for version)

This software depends on the fmt lib (MIT License),
and users must comply to its license: https://github.com/fmtlib/fmt/blob/main/LICENSE.rst

# Building

Please use lsstts/mtm1m3_sim Docker container to run simulator.

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

The base cRIO must be configured and up and running. ts_sal needs to be setup,
so SAL genetrated libraries (libsacpp_MTM1M3_types.so
libsacpp_MTMount_types.so  libSAL_MTM1M3.so  libSAL_MTMount.so) are in
LD_LIBRARY_PATH.

ExpansionFPGA resource name (set in NI MAX, under resource name) must match the
name specified in ExpansionFPGApplicationSettings under resource. This URI also
contains expansion cRIO IP address:

rio://139.229.178.185/M1M3-SupportExpansion-RIO

## Compiling for debugging

Please supply "DEBUG=1" to make call to compile with debug data (-g flag).

**Please be avare that cRIO memory usually isn't sufficient to compile code
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
make deploy
```

cRIO IP can be supplied with CRIO_IP=xx.yy.zz.ww argument.

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
work, HP and mirror position is not to be trusted in simulator as mirror is
raised or lowered. HP are send to target position after raise is completed
(mirror weight is 100% completed), so mirror position is reported correctly
when mirror is in active state.

## Running in deployment

ts_sal must be installed/available on target host. FPGA bitfiles with matching
signature shall be installed in Bitfiles directory. Those contains compiled
FPGA code, so you don't need to load (deploy) the code from LabView. This will
run only on cRIO system.

```bash
source ./ts_sal/setup.env;./ts_m1m3support/ts_M1M3Support -c SettingFiles
```

## Running m1m3cli

M1M3 command line client can be run as:

```bash
./m1m3cli
Please type help for more help.
M1M3 > open
[2020-06-09 20:12:50.964] [m1m3cli] [debug] FPGA: FPGA()
[2020-06-09 20:12:50.964] [m1m3cli] [debug] FPGA: initialize()
[2020-06-09 20:12:50.966] [m1m3cli] [debug] FPGA: open()
```

SAL isn't needed to run the command line tool.

## Running in simulation

After make SIMULATOR=1, you can run the code as simulator. This doesn't need
any hardware attached.

```bash
source ./ts_sal/setup.env;./ts_m1m3support/ts_M1M3Support -c SettingFiles
```

The simulator does a pretty good job giving realistic data, you will notice
some data isn't populated, it may be added at a later date, it may not. When
issuing state change commands make sure to specify "value" as otherwise the
command will be rejected.
