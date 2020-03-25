#include "XMLDocLoad.h"
#include <stdexcept>

using namespace pugi;

namespace LSST {
namespace M1M3 {
namespace SS {

void XMLDocLoad(const std::string &filename, xml_document &doc) {
    xml_parse_result res = doc.load_file(filename.c_str());
    if (res.status != pugi::xml_parse_status::status_ok) {
        throw std::runtime_error("Cannot load " + filename + ": " + res.description());
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
