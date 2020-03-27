#include <IExpansionFPGA.h>

#ifdef SIMULATOR
#include <SimulatedExpansionFPGA.h>
#else
#include <ExpansionFPGA.h>
#endif

using namespace LSST::M1M3::SS;

IExpansionFPGA& IExpansionFPGA::get() {
#ifdef SIMULATOR
    static SimulatedExpansionFPGA sexpansionfpga;
    return sexpansionfpga;
#else
    static ExpansionFPGA expansionfpga;
    return expansionfpga;
#endif
}
