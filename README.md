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

Any miss-communication, unexpected behaviour of either hardpoint, SAA or DAA
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
* *Controller* - Contains the C++ code for the ts\_M1M3Support application
* *Tests* - Contains python tests for the ts\_M1M3Support application
* *Utilities* - Contains the code for the utility applications

# Building

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

## Compiling for deployment

```bash
cd ${BASE}/ts_m1m3support/Controller/CentOS
make clean
make
```

## Compiling for simulation (TODO currently broken)

```bash
cd $BASE
source ts_sal/setup.env
cd ts_m1m3support/Controller/CentOS
make clean
make -DSIMULATOR
```

## Setting up the runtime environment

```bash
sudo mkdir /usr/ts_M1M3Support
sudo chmod a+rxw /usr/ts_M1M3Support
cp -r ./ts_m1m3support/Controller/SettingFiles /usr/ts_M1M3Support
```

## Running in deployment

```bash
source ./ts_sal/setup.env;ulimit -u unlimited;./ts_m1m3support/Controller/CentOS/ts_M1M3Support;
```

## Running in simulation (TODO currently broken)

```bash
source ./ts_sal/setup.env;./ts_m1m3support/Controller/CentOS/ts_M1M3Support
```

When issuing the start command specify the settings as Simulator The simulator
does a pretty good job giving realistic data, you will notice some data isn't
populated, it may be added at a later date, it may not. When issuing state
change commands make sure to specify "value" as 1 otherwise the command will be
rejected.
