#include <IBusList.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IBusList::~IBusList() {}

int32_t IBusList::getLength() { return 0; }
uint16_t* IBusList::getBuffer() { return 0; }

int32_t* IBusList::getExpectedHPResponses() { return 0; }
int32_t* IBusList::getExpectedFAResponses() { return 0; }
int32_t* IBusList::getExpectedHMResponses() { return 0; }

void IBusList::update() {}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
