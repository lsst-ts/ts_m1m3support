/*
 * IExpansionFPGA.cpp
 *
 *  Created on: Jan 9, 2018
 *      Author: ccontaxis
 */

#include <IExpansionFPGA.h>

namespace LSST {
namespace M1M3 {
namespace SS {
IExpansionFPGA::~IExpansionFPGA() { }

int32_t IExpansionFPGA::initialize() { return 0; }
int32_t IExpansionFPGA::open() { return 0; }
int32_t IExpansionFPGA::close() { return 0; }
int32_t IExpansionFPGA::finalize() { return 0; }

bool IExpansionFPGA::isErrorCode(int32_t status) { return false; }

int32_t IExpansionFPGA::sample() { return 0; }

int32_t IExpansionFPGA::readSlot1(float* data) { return 0; }
int32_t IExpansionFPGA::readSlot2(uint32_t* data) { return 0; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
