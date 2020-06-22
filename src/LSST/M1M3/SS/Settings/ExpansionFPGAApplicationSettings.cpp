#include <ExpansionFPGAApplicationSettings.h>
#include <XMLDocLoad.h>
#include <boost/lexical_cast.hpp>

using namespace pugi;

namespace LSST {
namespace M1M3 {
namespace SS {

void ExpansionFPGAApplicationSettings::load(const std::string &filename) {
    xml_document doc;
    XMLDocLoad(filename.c_str(), doc);
    this->Enabled =
            boost::lexical_cast<uint32_t>(
                    doc.select_node("//ExpansionFPGAApplicationSettings/Enabled").node().child_value()) != 0;
    this->Resource = doc.select_node("//ExpansionFPGAApplicationSettings/Resource").node().child_value();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
