# M1M3 Support Software

Contains soft real-time software which runs on NI cRIO for M1M3 support.
[FPGA](https://github.com/lsst-ts/ts_m1m3supportFPGA) handling low-level
communication, and 
[LabView EUI (GUI panel)](https://github.com/lsst-ts/ts_m1m3supportEUI) running
on host PC are separate projects.

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
