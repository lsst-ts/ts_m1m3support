#include <IFPGA.h>

#ifdef SIMULATOR
#include <SimulatedFPGA.h>
#else
#include <FPGA.h>
#endif

using namespace LSST::M1M3::SS;

IFPGA& IFPGA::get() {
#ifdef SIMULATOR
    static SimulatedFPGA simulatedfpga;
    return simulatedfpga;
#else
    static FPGA fpga;
    return fpga;
#endif
}
