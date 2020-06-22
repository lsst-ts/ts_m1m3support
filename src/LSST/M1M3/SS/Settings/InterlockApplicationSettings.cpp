#include <InterlockApplicationSettings.h>
#include <XMLDocLoad.h>
#include <boost/lexical_cast.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void InterlockApplicationSettings::load(const std::string &filename) {
    pugi::xml_document doc;
    XMLDocLoad(filename.c_str(), doc);

    this->HeartbeatPeriodInSeconds = boost::lexical_cast<double>(
            doc.select_node("//InterlockApplicationSettings/HeartbeatPeriodInSeconds").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
