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
